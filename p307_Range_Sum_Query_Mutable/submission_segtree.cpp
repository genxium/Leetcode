bool debug = false;
bool debugRangeAdd = false;
int const INVALID = -1;

class SegTreeNode {
public:
    int fullCoverAccDiff = 0; // The "full cover" add exactly performed or accumulated at the current node, WOULDN'T aggregate from any child and instead will be PUSHED DOWN to children
    int pSum = 0; // partial sum within [l, r), COULD BE aggregated from children
    int l, r; // "l" is closed index while "r" is open index
    SegTreeNode *lChild = NULL, *rChild = NULL;
    SegTreeNode(int aL, int aR): l(aL), r(aR) {}
    SegTreeNode* getOrCreateLChild() { return lChild ? lChild : lChild = new SegTreeNode(l, ((l+r) >> 1)); }
    SegTreeNode* getOrCreateRChild() { return rChild ? rChild : rChild = new SegTreeNode(((l+r) >> 1), r); }

    void RangeAdd(int aL, int aR, int unifiedDiff, int level) {
        // Reject invalid "[aL, aR)"s. 
        if (aL >= aR || aL >= r || aR <= l) return;

        int indentSpaceCount = (level << 2);
        if (debug & debugRangeAdd) printf("%*sRangeAdd, adding [%d, %d):%d into [%d, %d):%d.\n", indentSpaceCount, "", aL, aR, unifiedDiff, l, r, fullCoverAccDiff);

        if (aL <= l && r <= aR) {
            // Proactively stops at "full cover" update.
            accumulateOnlyHere(unifiedDiff);
        } else {
            // [REMINDER] The order of the following statements is important, i.e. "pushDownAtMostOneLevel" MUST be called before the recursive "child->RangeAdd" statements. 
            pushDownAtMostOneLevel(); 
            getOrCreateLChild()->RangeAdd(aL, aR, unifiedDiff, level+1);
            getOrCreateRChild()->RangeAdd(aL, aR, unifiedDiff, level+1);
            pSum = getOrCreateLChild()->pSum + getOrCreateRChild()->pSum;
        }

        if (debug & debugRangeAdd) printf("%*sRangeAdd, [%d, %d) updated to (fullCoverAccDiff:%d, pSum:%d).\n", indentSpaceCount, "", l, r, fullCoverAccDiff, pSum);
    }

    int RangeSum(int aL, int aR, int level) {
        // Reject invalid "[aL, aR)"s. 
        int indentSpaceCount = (level << 2);
        if (aL >= aR || aL >= r || aR <= l) return 0;
        if (aL <= l && r <= aR) {
            // Proactively stops at "full cover" update.
            if (debug) printf("%*sRangeSum#1, [%d, %d) has (fullCoverAccDiff:%d, pSum:%d).\n", indentSpaceCount, "", l, r, fullCoverAccDiff, pSum);
            return pSum;
        } else {
            // [REMINDER] The order of the following statements is important, i.e. "pushDownAtMostOneLevel" MUST be called before the recursive "child->RangeSum" statements. 
            pushDownAtMostOneLevel();
            int ret = 0;
            if (lChild) ret += lChild->RangeSum(aL, aR, level+1);    
            if (rChild) ret += rChild->RangeSum(aL, aR, level+1);
            if (debug) printf("%*sRangeSum#2, [%d, %d) has (fullCoverAccDiff:%d, pSum:%d, ret:%d).\n", indentSpaceCount, "", l, r, fullCoverAccDiff, pSum, ret);
            return ret;
        }
    }

    ~SegTreeNode() {
        if (lChild) delete lChild;
        if (rChild) delete rChild;
    }

private:
    void accumulateOnlyHere(int unifiedDiff) {
        pSum += (r-l)*unifiedDiff;
        fullCoverAccDiff += unifiedDiff;
    }

    void pushDownAtMostOneLevel() {
        if (lChild) lChild->accumulateOnlyHere(fullCoverAccDiff);    
        if (rChild) rChild->accumulateOnlyHere(fullCoverAccDiff);
        fullCoverAccDiff = 0; // clear
    }
};

class NumArray {
public:
    SegTreeNode *root = NULL;

    NumArray(vector<int>& nums) {
        root = new SegTreeNode(0, nums.size());
        for (int i = 0; i < nums.size(); ++i) root->RangeAdd(i, i+1, nums[i], 0);
    }
    
    void update(int i, int val) {
        int oldVal = root->RangeSum(i, i+1, 0);
        int diff = val-oldVal;
        root->RangeAdd(i, i+1, diff, 0);
    }
    
    int sumRange(int left, int right) {
        return root->RangeSum(left, right+1, 0);
    }

    ~NumArray() {
        delete root;
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(index,val);
 * int param_2 = obj->sumRange(left,right);
 */
