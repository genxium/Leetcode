bool debug = false;
bool debugRangeAdd = false;
typedef uint64_t ULL;
int const INVALID = -1;
ULL const MAXPOS = 100000;

// See https://github.com/genxium/Leetcode/tree/master/p850_Rectangle_Area_II for introductory information on segment tree. We're deliberately NOT USING "RangeSum" to speed up the solution!
class SegTreeNode {
public:
    int fullCoverAccDiff; // The full cover count exactly performed at the current node, WOULDN'T aggregate from any child. 
    int activeBottomLengthSum;
    int leftIndexClosed, rightIndexOpen;
    SegTreeNode *lChild, *rChild;

    SegTreeNode(int lClosed, int rOpen): fullCoverAccDiff(0), activeBottomLengthSum(0), leftIndexClosed(lClosed), rightIndexOpen(rOpen), lChild(NULL), rChild(NULL) {}

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

    void RangeAdd(int newSegLeftIndexClosed, int newSegRightIndexOpen, int unifiedDiff, int level) {
        // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
        if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
        if (newSegLeftIndexClosed >= rightIndexOpen) return;
        if (newSegRightIndexOpen <= leftIndexClosed) return;

        int indentSpaceCount = (level << 1);
        if (debug & debugRangeAdd) printf("%*sRangeAdd, adding [%d, %d):%d into [%d, %d):%d.\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, leftIndexClosed, rightIndexOpen, fullCoverAccDiff);

        if (newSegLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= newSegRightIndexOpen) {
            // Proactively stops at "full cover" update.
            fullCoverAccDiff += unifiedDiff;
        } else {
            getOrCreateLChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);    
            getOrCreateRChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);
        }

        if (0 < fullCoverAccDiff) {
            activeBottomLengthSum = rightIndexOpen-leftIndexClosed;
        } else {
            activeBottomLengthSum = 0;
            if (NULL != lChild) {
                activeBottomLengthSum += lChild->activeBottomLengthSum; 
            }

            if (NULL != rChild) {
                activeBottomLengthSum += rChild->activeBottomLengthSum;
            }
        }
        
        if (debug & debugRangeAdd) printf("%*sRangeAdd, [%d, %d) updated to (fullCoverAccDiff:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, fullCoverAccDiff);
    }
};

/*
test cases
[[1,1,3,3],[3,1,4,2],[3,2,4,4],[1,3,2,4],[2,3,3,4]]
[[1,1,2,3],[1,3,2,4],[3,1,4,2],[3,2,4,4]]
[[1,1,3,3],[3,1,4,2],[1,3,2,4],[3,2,4,4]]
[[1,1,3,3],[3,1,4,2],[1,3,2,4],[2,2,4,4]]
[[0,0,4,1],[7,0,8,2],[6,2,8,3],[5,1,6,3],[4,0,5,1],[6,0,7,2],[4,2,5,3],[2,1,4,3],[0,1,2,2],[0,2,2,3],[4,1,5,2],[5,0,6,1]]
[[-100000,-100000,100000,100000]]
[[0,0,2,2],[1,1,3,3],[2,0,3,1],[0,3,3,4]]
*/
class Solution {
public:
    bool isRectangleCover(vector<vector<int>>& rectangles) {
        ULL rawAreaSum = 0;
        int minL = INT_MAX, maxR = INT_MIN, minB = INT_MAX, maxT = INT_MIN;
        vector<vector<int>> edges;
        for (auto &rectangle : rectangles) {
            rawAreaSum += (ULL)abs(rectangle[0] - rectangle[2])*(ULL)abs(rectangle[1] - rectangle[3]);
            vector<int> bottomEdge = {rectangle[0], rectangle[2], +1, rectangle[1]};
            edges.push_back(bottomEdge);
            vector<int> topEdge = {rectangle[0], rectangle[2], -1, rectangle[3]};
            edges.push_back(topEdge);
            minL = min(rectangle[0], minL);
            maxR = max(rectangle[2], maxR);
            minB = min(rectangle[1], minB);
            maxT = max(rectangle[3], maxT);
        }
        ULL hullArea = (ULL)abs(maxT - minB)*(ULL)abs(maxR - minL);
        if (debug) printf("rawAreaSum: %lu, minL: %d, maxR: %d\n", rawAreaSum, minL, maxR);

        sort(edges.begin(), edges.end(), [](vector<int> const& lhs, vector<int> const& rhs) {
            if (lhs[3] != rhs[3]) return lhs[3] < rhs[3];
            return lhs[2] > rhs[2]; // [WARNING] Bottom edge comes first!
        });

        SegTreeNode* root = new SegTreeNode(minL, maxR);

        ULL mergedArea = 0;
        int lastY = INT_MAX, expectedActiveBottomLength = (maxR - minL);
        for (auto &edge : edges) {
            int currentY = edge[3];
            if (INT_MAX != lastY && currentY != lastY) {
                // [TRICK] only check this condition when "lastY" is updated 
                if (root->activeBottomLengthSum != expectedActiveBottomLength) {
                    return false;
                }
                mergedArea += (ULL)(currentY-lastY)*expectedActiveBottomLength;
                if (debug) printf("lastY: %d, currentY:%d\n", lastY, currentY);
            }
            root->RangeAdd(edge[0], edge[1], edge[2], 0);
            lastY = currentY;
        }

        if (debug) printf("mergedArea = %lu\n", mergedArea);
        return (mergedArea == rawAreaSum && mergedArea == hullArea);
    }
};
