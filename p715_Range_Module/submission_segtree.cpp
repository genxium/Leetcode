bool debug = false;
int const MAXPOS = 1000000000; // 1,000,000,000
int const INVALID = -1;

// See https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e for an introduction to SegmentTree properties.
class SegTreeNode {
public:
    int unifiedValue; // INVALID, false, true
    int leftIndexClosed;
    int rightIndexOpen;
    SegTreeNode* lChild;
    SegTreeNode* rChild;

    SegTreeNode(int lClosed, int rOpen) {
        unifiedValue = INVALID;
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

    int RangeQuery(int targetLeftIndexClosed, int targetRightIndexOpen, int level) {
        // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
        if (targetLeftIndexClosed >= targetRightIndexOpen) return false;
        // [WARNING] If the current node is totally not touched, it should return true!
        if (targetLeftIndexClosed >= rightIndexOpen) return true; 
        if (targetRightIndexOpen <= leftIndexClosed) return true;

        if (INVALID != unifiedValue) {
            return unifiedValue;
        }
        
        int lCand = INVALID, rCand = INVALID;
        if (NULL != lChild) {
            lCand = lChild->RangeQuery(targetLeftIndexClosed, targetRightIndexOpen, level+1);
        }

        if (NULL != rChild) {
            rCand = rChild->RangeQuery(targetLeftIndexClosed, targetRightIndexOpen, level+1);
        }

        return ((true == lCand) && (true == rCand));
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
            if (debug) printf("%*sRangeWrite, [%d, %d) updated to (unifiedValue:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, unifiedValue);
            return;
        }

        if (newUnifiedValue == unifiedValue) return;

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
        
        if (INVALID != lChild->unifiedValue && lChild->unifiedValue == rChild->unifiedValue) {
            unifiedValue = lChild->unifiedValue;    
        } // Otherwise "this->unifiedValue" remains INVALID.
    }
};

/*
test cases
["RangeModule","addRange","removeRange","queryRange","queryRange","queryRange"]
[[],[10,20],[14,16],[10,14],[13,15],[16,17]]
["RangeModule","addRange","removeRange","queryRange","queryRange","queryRange","queryRange","queryRange","queryRange","queryRange"]
[[],[1,10],[4,6],[1,5],[1,6],[1,7],[4,5],[4,6],[4,7],[6,7]]
*/
class RangeModule {
private:
    SegTreeNode* root = NULL;
public:
    RangeModule() {
        root = new SegTreeNode(0, MAXPOS);
    }
    
    void addRange(int left, int right) {
        root->RangeWrite(left, right, true, true, 0);
    }
    
    bool queryRange(int left, int right) {
        int tmp = root->RangeQuery(left, right, 0);
        return (tmp == true);
    }
    
    void removeRange(int left, int right) {
        root->RangeWrite(left, right, false, true, 0);
    }
};

/**
 * Your RangeModule object will be instantiated and called as such:
 * RangeModule* obj = new RangeModule();
 * obj->addRange(left,right);
 * bool param_2 = obj->queryRange(left,right);
 * obj->removeRange(left,right);
 */
