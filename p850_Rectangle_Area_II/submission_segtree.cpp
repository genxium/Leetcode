bool debug = false;
bool debugRangeAdd = false;
typedef uint64_t ULL;
int const INVALID = -1;
ULL const MOD = 1000000007u;
ULL const MAXPOS = 1000000000;

// See https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e for an introduction to SegmentTree properties.
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

    int RangeSum(int targetLeftIndexClosed, int targetRightIndexOpen, int level) {
        // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
        if (targetLeftIndexClosed >= targetRightIndexOpen) return 0;
        if (targetLeftIndexClosed >= rightIndexOpen) return 0;
        if (targetRightIndexOpen <= leftIndexClosed) return 0;
    
        int indentSpaceCount = (level << 1);
        
        if (fullCoverAccDiff > 0) {
            return (min(targetRightIndexOpen, rightIndexOpen)-max(targetLeftIndexClosed, leftIndexClosed));
        }

        // 0 == fullCoverAccDiff, aggregate from children
        bool currentNodeFullyCovered = (targetLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= targetRightIndexOpen);
    
        int toRet = 0;
        if (NULL != lChild) {
            int cand = lChild->RangeSum(targetLeftIndexClosed, targetRightIndexOpen, level+1); 
            toRet += cand;
        }

        if (NULL != rChild) {
            int cand = rChild->RangeSum(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet += cand;
        }

        if (currentNodeFullyCovered) {
            activeBottomLengthSum = toRet;
        }

        return toRet;
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
            if (0 < fullCoverAccDiff) {
                activeBottomLengthSum = (rightIndexOpen-leftIndexClosed);
            } else {
                // [WARNING] In this case, "activeBottomLengthSum" might be later updated by "RangeSum"!
                activeBottomLengthSum = INVALID;
            }
            if (debug & debugRangeAdd) printf("%*sRangeAdd, [%d, %d) updated to (fullCoverAccDiff:%d, activeBottomLengthSum:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, fullCoverAccDiff, activeBottomLengthSum);
            return;
        }
        // [WARNING] In this case, "activeBottomLengthSum" might be later updated by "RangeSum"!
        activeBottomLengthSum = INVALID;
        getOrCreateLChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);    
        getOrCreateRChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);

        if (debug & debugRangeAdd) printf("%*sRangeAdd, [%d, %d) updated to (fullCoverAccDiff:%d, activeBottomLengthSum:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, fullCoverAccDiff, activeBottomLengthSum);
    }
};

/*
test cases
[[0,0,2,2],[1,0,2,3],[1,0,3,1]]
[[0,0,1000000000,1000000000]]
[[0,0,2,2],[1,1,3,3]]
[[0,0,3,3],[2,0,5,3],[1,1,4,4]]
*/
class Solution {
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        vector<vector<int>> edges;
        for (auto &rectangle : rectangles) {
            vector<int> bottomEdge = {rectangle[0], rectangle[2], +1, rectangle[1]};
            edges.push_back(bottomEdge);
            vector<int> topEdge = {rectangle[0], rectangle[2], -1, rectangle[3]};
            edges.push_back(topEdge);
        }

        sort(edges.begin(), edges.end(), [](vector<int> const& lhs, vector<int> const& rhs) {
            if (lhs[3] != rhs[3]) return lhs[3] < rhs[3];
            return lhs[2] > rhs[2]; // bottomEdge comes first
        });

        SegTreeNode* root = new SegTreeNode(0, MAXPOS);

        ULL mergedArea = 0;
        int lastY = 0;
        for (auto &edge : edges) {
            int currentY = edge[3];
            if (currentY != lastY) {
                /*
                Note that We're computing "activeBottomLengthSum" regardless of "edge[0], edge[1]".
                */
                int activeBottomLengthSum = root->RangeSum(0, MAXPOS, 0);
                if (debug) printf("lastY: %d, currentY: %d, activeBottomLengthSum:%d\n", lastY, currentY, activeBottomLengthSum);
                mergedArea += (ULL)(currentY-lastY)*(ULL)activeBottomLengthSum;
                mergedArea %= MOD;
            }
            root->RangeAdd(edge[0], edge[1], edge[2], 0);
            lastY = currentY;
        }

        return mergedArea;
    }
};
