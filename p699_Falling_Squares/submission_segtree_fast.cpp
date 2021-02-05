bool debug = false;
int const MAXPOS = 101000001; // 101000001 == 100,000,000 + 1,000,000

int const INVALID = -1;

// See https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e for an introduction to SegmentTree properties.
class SegTreeNode {
public:
    int unifiedValue; // a.k.a. "unified height in x-range [leftIndexClosed, rightIndexOpen)"
    int highestValue; // To speed up query!
    int leftIndexClosed;
    int rightIndexOpen;
    SegTreeNode* lChild;
    SegTreeNode* rChild;

    SegTreeNode(int lClosed, int rOpen) {
        unifiedValue = INVALID;
        highestValue = INVALID;
        leftIndexClosed = lClosed;
        rightIndexOpen = rOpen;
        lChild = rChild = NULL;
    }

    int calcMid() {
        return ((leftIndexClosed+rightIndexOpen) >> 1);
    }

    SegTreeNode* getOrCreateLChild() {
        if (NULL == lChild) {
            int mid = calcMid();
            lChild = new SegTreeNode(leftIndexClosed, mid);
        }
        return lChild;
    }

    SegTreeNode* getOrCreateRChild() {
        if (NULL == rChild) {
            int mid = calcMid();
            rChild = new SegTreeNode(mid, rightIndexOpen);
        }
        return rChild;
    }

    int RangeMax(int targetLeftIndexClosed, int targetRightIndexOpen, int level) {
        // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
        if (targetLeftIndexClosed >= targetRightIndexOpen) return 0;
        if (targetLeftIndexClosed >= rightIndexOpen) return 0;
        if (targetRightIndexOpen <= leftIndexClosed) return 0;

        if (INVALID != unifiedValue) {
            return unifiedValue;
        }
    
        bool currentNodeFullyCovered = (targetLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= targetRightIndexOpen); 
        if (currentNodeFullyCovered) {
            if (INVALID != highestValue) {
                return highestValue;
            }
        }

        int toRet = 0;
        if (NULL != lChild) {
            int cand = lChild->RangeMax(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet = max(toRet, cand);
        }

        if (NULL != rChild) {
            int cand = rChild->RangeMax(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet = max(toRet, cand);
        }

        if (currentNodeFullyCovered) {
            // [WARNING] If "false == currentNodeFullyCovered", DON'T assign "toRet" to "this->highestValue"!
            highestValue = toRet;
        }

        return toRet;
    }

    void RangeWrite(int newSegLeftIndexClosed, int newSegRightIndexOpen, int newUnifiedValue, bool allowResiduePropagation, int level) {
        // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
        if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
        if (newSegLeftIndexClosed >= rightIndexOpen) return;
        if (newSegRightIndexOpen <= leftIndexClosed) return;

        int indentSpaceCount = (level << 1);
        if (debug) printf("%*sRangeWrite, assigning [%d, %d):%d into [%d, %d):%d, allowResiduePropagation = %d.\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, newUnifiedValue, leftIndexClosed, rightIndexOpen, unifiedValue, allowResiduePropagation);

        if (newSegLeftIndexClosed <= leftIndexClosed && newSegRightIndexOpen >= rightIndexOpen) {
            // Proactively stops at "full cover" update.
            unifiedValue = newUnifiedValue;
            highestValue = newUnifiedValue;
            if (debug) printf("%*sRangeWrite, [%d, %d) updated to (unifiedValue:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, unifiedValue);
            return;
        }

        if (newUnifiedValue == unifiedValue) {
            // Prohibits a larger value to be written in.
            return;
        }

        int residueUnifiedValue = unifiedValue;
        unifiedValue = INVALID; // By now it's certain that "this->unifiedValue" is no longer unified.

        if (true == allowResiduePropagation && INVALID != residueUnifiedValue) {
            /*
            * Having "INVALID != residueUnifiedValue" implies that it's "more recent" than "any residueUnifiedValue from the subtree".
            */
            int leftResidueLeftClosed = leftIndexClosed, leftResidueRightOpen = newSegLeftIndexClosed;
            int rightResidueLeftClosed = newSegRightIndexOpen, rightResidueRightOpen = rightIndexOpen;
            getOrCreateLChild()->RangeWrite(leftResidueLeftClosed, leftResidueRightOpen, residueUnifiedValue, false, level+1);
            getOrCreateLChild()->RangeWrite(rightResidueLeftClosed, rightResidueRightOpen, residueUnifiedValue, false, level+1); 
            getOrCreateRChild()->RangeWrite(leftResidueLeftClosed, leftResidueRightOpen, residueUnifiedValue, false, level+1);
            getOrCreateRChild()->RangeWrite(rightResidueLeftClosed, rightResidueRightOpen, residueUnifiedValue, false, level+1);
        }

        getOrCreateLChild()->RangeWrite(newSegLeftIndexClosed, newSegRightIndexOpen, newUnifiedValue, allowResiduePropagation, level+1);
        if (debug) printf("%*sRangeWrite, [%d, %d) returned from lChild.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen);
        
        getOrCreateRChild()->RangeWrite(newSegLeftIndexClosed, newSegRightIndexOpen, newUnifiedValue, allowResiduePropagation, level+1);
        if (debug) printf("%*sRangeWrite, [%d, %d) returned from rChild.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen);
        
        highestValue = max(lChild->highestValue, rChild->highestValue);
    }
};

class Solution {
public:
    vector<int> fallingSquares(vector<vector<int>>& positions) {
        SegTreeNode* root = new SegTreeNode(1, MAXPOS+1);
        vector<int> ans;
        for (auto &p : positions) {
            int l = p[0], r = l+p[1];
            int lastHInRange = root->RangeMax(l, r, 0);
            int newH = lastHInRange + p[1];
            root->RangeWrite(l, r, newH, true, 0);
            int maxOverallH = root->RangeMax(0, MAXPOS+1, 0);
            ans.push_back(maxOverallH);
        }

        return ans;
    }
};
