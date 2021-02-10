bool const debug = false;
int const INVALID = -1;
int const MAXPOS = 1000000000;

// See https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e for an introduction to SegmentTree properties.
class SegTreeNode {
public:
    int fullCoverAccDiff; // The full cover count exactly performed at the current node, WOULDN'T aggregate from any child. 
    int maxCoverCount; // The max cover count of each number within [leftIndexClosed, rightIndexOpen), WILL aggregate from children.
    int leftIndexClosed;
    int rightIndexOpen;
    SegTreeNode* lChild;
    SegTreeNode* rChild;

    SegTreeNode(int lClosed, int rOpen) {
        fullCoverAccDiff = 0;
        maxCoverCount = 0;
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

    int RangeMaxCoverCount(int targetLeftIndexClosed, int targetRightIndexOpen, int level) {
        // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
        if (targetLeftIndexClosed >= targetRightIndexOpen) return 0;
        if (targetLeftIndexClosed >= rightIndexOpen) return 0;
        if (targetRightIndexOpen <= leftIndexClosed) return 0;
    
        bool currentNodeFullyCovered = (targetLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= targetRightIndexOpen); 
        if (currentNodeFullyCovered) {
            return maxCoverCount;
        }

        int toRet = 0;
        if (NULL != lChild) {
            int cand = lChild->RangeMaxCoverCount(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet = max(toRet, cand);
        }

        if (NULL != rChild) {
            int cand = rChild->RangeMaxCoverCount(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet = max(toRet, cand);
        }

        return toRet;
    }

    void RangeAdd(int newSegLeftIndexClosed, int newSegRightIndexOpen, int unifiedDiff, int level) {
        // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
        if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
        if (newSegLeftIndexClosed >= rightIndexOpen) return;
        if (newSegRightIndexOpen <= leftIndexClosed) return;

        int indentSpaceCount = (level << 1);
        if (debug) printf("%*sRangeAdd, adding [%d, %d):%d into [%d, %d):%d.\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, leftIndexClosed, rightIndexOpen, fullCoverAccDiff);

        if (newSegLeftIndexClosed <= leftIndexClosed && newSegRightIndexOpen >= rightIndexOpen) {
            // Proactively stops at "full cover" update.
            fullCoverAccDiff += unifiedDiff;
            maxCoverCount += unifiedDiff;
            if (debug) printf("%*sRangeAdd, [%d, %d) updated to (fullCoverAccDiff:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, fullCoverAccDiff);
            return;
        }

        getOrCreateLChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);
        if (debug) printf("%*sRangeAdd, [%d, %d) returned from lChild.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen);
        
        getOrCreateRChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);
        if (debug) printf("%*sRangeAdd, [%d, %d) returned from rChild.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen);
        
        int lCand = lChild->maxCoverCount + fullCoverAccDiff; // "this->fullCoverAccDiff" is not included in "lChild->maxCoverCount"
        int rCand = rChild->maxCoverCount + fullCoverAccDiff;
        
        int cand = max(lCand, rCand); 
        maxCoverCount = max(maxCoverCount, cand);
    }
};

class MyCalendarThree {
private:
    SegTreeNode *root = NULL;
public:
    MyCalendarThree() {
        root = new SegTreeNode(0, MAXPOS);
    }
    
    int book(int start, int end) {
        root->RangeAdd(start, end, 1, 0);
        int k = root->RangeMaxCoverCount(0, MAXPOS, 0);
        return k;
    }
};

/**
 * Your MyCalendarThree object will be instantiated and called as such:
 * MyCalendarThree* obj = new MyCalendarThree();
 * int param_1 = obj->book(start,end);
 */
