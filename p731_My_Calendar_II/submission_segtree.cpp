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
    
        int indentSpaceCount = (level << 1);
        
        bool currentNodeFullyCovered = (targetLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= targetRightIndexOpen); 
        if (currentNodeFullyCovered) {
            if (debug) printf("%*s#1,RangeMaxCoverCount, [%d, %d) returning %d.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, maxCoverCount);
            return maxCoverCount;
        }

        int toRet = fullCoverAccDiff;
        if (NULL != lChild) {
            int cand = lChild->RangeMaxCoverCount(targetLeftIndexClosed, targetRightIndexOpen, level+1); 
            toRet = max(toRet, cand+fullCoverAccDiff); // "this->fullCoverAccDiff" is not included in "lChild->RangeMaxCoverCount(...)"
        }

        if (NULL != rChild) {
            int cand = rChild->RangeMaxCoverCount(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet = max(toRet, cand+fullCoverAccDiff);
        }

        if (debug) printf("%*s#2, RangeMaxCoverCount, [%d, %d) returning %d.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, toRet);
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
            // if (debug) printf("%*sRangeAdd, [%d, %d) updated to (fullCoverAccDiff:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, fullCoverAccDiff);
            return;
        }

        getOrCreateLChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);
        // if (debug) printf("%*sRangeAdd, [%d, %d) returned from lChild.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen);
        
        getOrCreateRChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);
        // if (debug) printf("%*sRangeAdd, [%d, %d) returned from rChild.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen);
        
        int lCand = lChild->maxCoverCount + fullCoverAccDiff; // "this->fullCoverAccDiff" is not included in "lChild->maxCoverCount"
        int rCand = rChild->maxCoverCount + fullCoverAccDiff;
        
        int cand = max(lCand, rCand); 
        maxCoverCount = max(maxCoverCount, cand);
        
        if (debug) printf("%*sRangeAdd, [%d, %d) returned with maxCoverCount:%d.\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, maxCoverCount);
    }
};

/*
test cases
["MyCalendarTwo","book","book","book","book","book","book"]
[[],[10,20],[50,60],[10,40],[5,15],[5,10],[25,55]]
["MyCalendarThree","book","book","book"]
[[],[27,36],[27,36],[23,28]]
["MyCalendarThree","book","book","book","book","book","book","book","book","book"]
[[],[47,50],[1,10],[27,36],[40,47],[20,27],[15,23],[10,18],[27,36],[23,28]]
["MyCalendarTwo","book","book","book","book","book","book","book","book","book","book","book","book"]
[[],[47,50],[1,10],[27,36],[40,47],[20,27],[15,23],[10,18],[27,36],[17,25],[8,17],[24,33],[23,28]]
["MyCalendarTwo","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book","book"]
[[],[47,50],[1,10],[27,36],[40,47],[20,27],[15,23],[10,18],[27,36],[17,25],[8,17],[24,33],[23,28],[21,27],[47,50],[14,21],[26,32],[16,21],[2,7],[24,33],[6,13],[44,50],[33,39],[30,36],[6,15],[21,27],[49,50],[38,45],[4,12],[46,50],[13,21]]
["MyCalendarTwo","book","book","book","book"]
[[],[28,46],[9,21],[21,39],[37,48]]
["MyCalendarTwo","book","book","book","book","book","book","book","book","book","book"]
[[],[28,46],[9,21],[21,39],[37,48],[38,50],[22,39],[45,50],[1,12],[40,50],[31,44]]
*/
class MyCalendarTwo {
private:
    SegTreeNode *root = NULL;
public:
    MyCalendarTwo() {
        root = new SegTreeNode(0, MAXPOS);
    }
    
    bool book(int start, int end) {
        if (debug) {
            printf("Checking k for [start:%d, end:%d)\n", start, end);
        }
        int k = root->RangeMaxCoverCount(start, end, 0);
        if (debug) {
            printf("k for [start:%d, end:%d) is %d\n", start, end, k);
        }
        if (2 == k) return false;
        root->RangeAdd(start, end, 1, 0);
        return true;
    }
};

/**
 * Your MyCalendarTwo object will be instantiated and called as such:
 * MyCalendarTwo* obj = new MyCalendarTwo();
 * bool param_1 = obj->book(start,end);
 */
