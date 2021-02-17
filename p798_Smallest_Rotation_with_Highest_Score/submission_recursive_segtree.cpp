bool const debug = false;
bool const debugRangeAdd = false;
bool const debugRangeSum = false;
int const INVALID = -1;
int const MAXPOS = 40000;

// See https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e for an introduction to SegmentTree properties.
class SegTreeNode {
public:
    int fullCoverAccDiff; // The full cover count exactly performed at the current node, WOULDN'T aggregate from any child. 
    int sum;
    int leftIndexClosed, rightIndexOpen;
    SegTreeNode *lChild, *rChild;

    SegTreeNode(int lClosed, int rOpen): fullCoverAccDiff(0), sum(0), leftIndexClosed(lClosed), rightIndexOpen(rOpen), lChild(NULL), rChild(NULL) {}

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
        
        bool currentNodeFullyCovered = (targetLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= targetRightIndexOpen); 
        if (currentNodeFullyCovered) {
            if (debug & debugRangeSum) {
                printf("%*sRangeSum, [%d, %d) returning %d\n", indentSpaceCount, "", targetLeftIndexClosed, targetRightIndexOpen, sum);
            }
            return sum;
        }

        int toRet = 0;
        if (NULL != lChild) {
            int cand = lChild->RangeSum(targetLeftIndexClosed, targetRightIndexOpen, level+1); 
            toRet += cand;
        }

        if (NULL != rChild) {
            int cand = rChild->RangeSum(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet += cand;
        }

        if (debug & debugRangeSum) {
            printf("%*sRangeSum, [%d, %d) returning %d\n", indentSpaceCount, "", targetLeftIndexClosed, targetRightIndexOpen, toRet);
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
            sum += unifiedDiff*(rightIndexOpen-leftIndexClosed);
            if (debug & debugRangeAdd) printf("%*sRangeAdd, [%d, %d) updated to (fullCoverAccDiff:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, fullCoverAccDiff);
            return;
        }

        getOrCreateLChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);    
        getOrCreateRChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedDiff, level+1);
        sum = getOrCreateLChild()->sum + getOrCreateRChild()->sum;
    }
};

/*
test cases
[2,3,1,4,0]
[2,4,1,3,0]
*/
class Solution {
public:
    int bestRotation(vector<int>& A) {
        /*
        For each "A[i < k]" the index becomes "n-k+i", for each "A[i >= k]" the index becomes "i-k", the goal is to maximize
        ```
        count(A[i] <= n-k+i, i < k) + count(A[i] <= i-k, i >= k)
        ```
        , which is equivalent to finding a "k" to maximize 
        ```
        count(A[i]-i-n <= -k, i < k) + count(A[i]-i <= -k, i >= k)
        ```
        , which in turn is solvable by "RangeQuery" on two arrays "foo={A[i]-i-n}" & "bar={A[i]-i}".
        
        An example
        [ 2,  3,   1,  4,  0 ]
         -3, -3,  -6, -4, -9 : A[i]-i-n
          2,  2,  -1,  1, -5 : A[i]-i
        */
        SegTreeNode* foo = new SegTreeNode(-MAXPOS, MAXPOS+1);
        SegTreeNode* bar = new SegTreeNode(-MAXPOS, MAXPOS+1);
        int n = A.size();
        if (debug) printf("n is %d\n", n);
        for (int i = 0; i < n; ++i) {
            bar->RangeAdd(A[i]-i, A[i]-i+1, 1, 0);
        }

        int ans = 0, maxScoreByFar = 0;
        for (int k = 0; k < n; ++k) {
            // By now only {A[i]+i-n, i < k} is in "foo".
            int lScore = foo->RangeSum(-MAXPOS, -k+1, 0);
            // By now only {A[i]-i, i >= k} is in "bar".
            int rScore = bar->RangeSum(-MAXPOS, -k+1, 0);
            int score = lScore+rScore;
            if (debug) printf("k:%d, lScore:%d, rScore:%d\n", k, lScore, rScore);
            if (score > maxScoreByFar) {
                ans = k;
                maxScoreByFar = score;
            }
            foo->RangeAdd(A[k]-k-n, A[k]-k-n+1, 1, 0);
            bar->RangeAdd(A[k]-k, A[k]-k+1, -1, 0);
        }

        return ans;
    }
};
