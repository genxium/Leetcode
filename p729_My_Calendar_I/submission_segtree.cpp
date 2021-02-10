bool const debug = false;
int const INVALID = -1;
int const MAXPOS = 1000000000;

// See https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e for an introduction to SegmentTree properties.
class SegTreeNode {
public:
    int unifiedValue;
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

    int RangeUnifiedValue(int targetLeftIndexClosed, int targetRightIndexOpen, int level) {
        // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
        if (targetLeftIndexClosed >= targetRightIndexOpen) return 0;
        if (targetLeftIndexClosed >= rightIndexOpen) return 0;
        if (targetRightIndexOpen <= leftIndexClosed) return 0;

        if (INVALID != unifiedValue) {
            return unifiedValue;
        }
    
        bool currentNodeFullyCovered = (targetLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= targetRightIndexOpen); 
        if (currentNodeFullyCovered) {
            return unifiedValue; // It might be INVALID
        }

        int toRet = INVALID;
        if (NULL != lChild) {
            int cand = lChild->RangeUnifiedValue(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet = cand;
        }

        if (NULL != rChild) {
            int cand = rChild->RangeUnifiedValue(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            if (cand != toRet) toRet = INVALID; // Regardless of what "toRet" was
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
        
        if (lChild->unifiedValue != INVALID && lChild->unifiedValue == rChild->unifiedValue) {
            unifiedValue = lChild->unifiedValue;
        }
    }
};


class MyCalendar {
private: 
    SegTreeNode *root = NULL;
public:
    MyCalendar() {
        root = new SegTreeNode(0, MAXPOS);
        root->RangeWrite(0, MAXPOS, 0, true, 0);
    }
    
    bool book(int start, int end) {
        int a = root->RangeUnifiedValue(start, end, 0);
        if (0 != a) return false;
        root->RangeWrite(start, end, 1, true, 0);
        return true;
    }
};

/**
 * Your MyCalendar object will be instantiated and called as such:
 * MyCalendar* obj = new MyCalendar();
 * bool param_1 = obj->book(start,end);
 */
