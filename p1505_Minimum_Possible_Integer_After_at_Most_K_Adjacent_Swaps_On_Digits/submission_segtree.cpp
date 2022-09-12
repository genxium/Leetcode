bool debug = true;
bool debugRangeWrite = false;
int const INVALID = -1;
int const MAXL = 30000;

class SegTreeNode {
public:
    int unifiedValue = INVALID; 
    int charMaxCount = 0;
    SegTreeNode* minFullCover = NULL; // By definition, if NULL != minFullCover, then INVALID != minFullCover->unifiedValue

    int l, r; // "l" is closed index while "r" is open index
    SegTreeNode *lChild = NULL, *rChild = NULL;
    SegTreeNode(int aL, int aR): l(aL), r(aR) {}
    SegTreeNode* getOrCreateLChild() { return lChild ? lChild : lChild = new SegTreeNode(l, ((l+r) >> 1)); }
    SegTreeNode* getOrCreateRChild() { return rChild ? rChild : rChild = new SegTreeNode(((l+r) >> 1), r); }

    void RangeWrite(int aL, int aR, int newUnifiedValue, bool allowResiduePropagation, int level) {
        // Reject invalid "[aL, aR)"s. 
        if (aL >= aR || aL >= r || aR <= l) return;

        int indentSpaceCount = (level << 2);
        if (debug && debugRangeWrite) printf("%*sRangeWrite, assigning [%d, %d):%d into [%d, %d):%d, allowResiduePropagation = %d.\n", indentSpaceCount, "", aL, aR, newUnifiedValue, l, r, unifiedValue, allowResiduePropagation);

        if (aL <= l && aR >= r) {
            // Proactively stops at "full cover" update.
            unifiedValue = newUnifiedValue;
            minFullCover = this; // all values in this fully covered range is the same
            if (CHAR_MAX == unifiedValue) charMaxCount = (r-l);
            if (debug && debugRangeWrite) printf("%*sRangeWrite, [%d, %d) updated to (unifiedValue:%d).\n", indentSpaceCount, "", l, r, unifiedValue);
            return;
        }

        if (newUnifiedValue == unifiedValue) return;

        int residueUnifiedValue = unifiedValue;
        charMaxCount = 0;
        unifiedValue = INVALID; // By now it's certain that "this->unifiedValue" is no longer unified.

        if (allowResiduePropagation && INVALID != residueUnifiedValue) {
            /*
            * Having "INVALID != residueUnifiedValue" implies that it's "more recent" than "any residueUnifiedValue from the subtree".
            */
            getOrCreateLChild()->RangeWrite(l, aL, residueUnifiedValue, false, level+1);
            getOrCreateLChild()->RangeWrite(aR, r, residueUnifiedValue, false, level+1); 
            getOrCreateRChild()->RangeWrite(l, aL, residueUnifiedValue, false, level+1);
            getOrCreateRChild()->RangeWrite(aR, r, residueUnifiedValue, false, level+1);
        }

        getOrCreateLChild()->RangeWrite(aL, aR, newUnifiedValue, allowResiduePropagation, level+1);        
        getOrCreateRChild()->RangeWrite(aL, aR, newUnifiedValue, allowResiduePropagation, level+1);        
        if (lChild->unifiedValue == rChild->unifiedValue) unifiedValue = lChild->unifiedValue;
        // update minFullCover
        if (!rChild->minFullCover) minFullCover = lChild->minFullCover;
        else if (!lChild->minFullCover) minFullCover = rChild->minFullCover;
        else minFullCover = (lChild->minFullCover->unifiedValue <= rChild->minFullCover->unifiedValue ? lChild->minFullCover : rChild->minFullCover);

        charMaxCount += lChild->charMaxCount;
        charMaxCount += rChild->charMaxCount;
    }

    SegTreeNode* RangeMin(int aL, int aR, int level) {
        if (aL >= aR || aL >= r || aR <= l) return NULL;

        int indentSpaceCount = (level << 2);
        if (aL <= l && aR >= r) {
            // At "full cover"
            return minFullCover; 
        }

        SegTreeNode* lResult = lChild->RangeMin(aL, aR, level+1);
        SegTreeNode* rResult = rChild->RangeMin(aL, aR, level+1);
        if (!rResult) return lResult;
        if (!lResult) return rResult;
        return (lResult->unifiedValue <= rResult->unifiedValue ? lResult : rResult); // "lResult" is of higher priority due to the greedy approach of this problem
    }

    int RangeCharMaxCount(int aL, int aR, int level) {
        if (aL >= aR || aL >= r || aR <= l) return 0;

        if (aL <= l && aR >= r) {
            // At "full cover"
            return charMaxCount; 
        }

        return lChild->RangeCharMaxCount(aL, aR, level+1) + rChild->RangeCharMaxCount(aL, aR, level+1);
    }
};

/*
test cases
"4321"
4
"9000900"
3
"9438957234785635408"
1000000000
"294984148179"
11
*/
class Solution {
    public:
        string minInteger(string num, int k) {
            string ans = "";
            int n = num.length();
            SegTreeNode* root = new SegTreeNode(0, n);
            for (int j = 0; j < n; ++j) root->RangeWrite(j, j+1, num[j], true, 0);
            int charMaxInReach = 0;
            int i = 0;
            while (k > 0 && i < n) {
                if (CHAR_MAX == num[i]) { ++i; continue; }
                // The "CHAR_MAX" slots shouldn't be counted as distance, thus an effective "reach" is computed ITERATIVELY
                int origReach = min(i+k+1, n), reach = origReach;
                int prevCharMaxCountInReach = 0, charMaxCountInReach = root->RangeCharMaxCount(i, reach, 0);
                while (charMaxCountInReach > prevCharMaxCountInReach) {
                    reach = origReach + charMaxCountInReach;
                    prevCharMaxCountInReach = charMaxCountInReach;
                    charMaxCountInReach = root->RangeCharMaxCount(i, reach, 0);
                }

                SegTreeNode* minFullCover = root->RangeMin(i, reach, 0); // GREEDILY find the minimum value within "reach"
                int j = minFullCover->l, jVal = minFullCover->unifiedValue; 
                ans.push_back(jVal);
                if (j == i) { ++i; continue; }
                k -= (j - i - root->RangeCharMaxCount(i, j, 0));
                num[j] = CHAR_MAX;
                root->RangeWrite(j, j+1, CHAR_MAX, true, 0);
            }

            for (int j = i; j < n; ++j) {
                if (CHAR_MAX == num[j]) continue;
                ans.push_back(num[j]);
            }

            return ans;
        }
};
