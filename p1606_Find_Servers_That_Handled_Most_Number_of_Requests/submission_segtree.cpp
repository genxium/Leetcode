int const INVALID = -1;
bool debug = false;
bool debugRangeUpdate = false;
bool debugRangeMin = false;

class SegTreeNode {
public:
    int minInRange;
    int leftIndexClosed, rightIndexOpen;
    SegTreeNode *lChild, *rChild;

    SegTreeNode(int lClosed, int rOpen): minInRange(0), leftIndexClosed(lClosed), rightIndexOpen(rOpen), lChild(NULL), rChild(NULL) {}

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

    void RangeUpdate(int newSegLeftIndexClosed, int newSegRightIndexOpen, int newMinInRange, int level) {
        // We're always having newSegRightIndexOpen == newSegLeftIndexClosed+1 in this problem.
        // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
        if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
        if (newSegLeftIndexClosed >= rightIndexOpen) return;
        if (newSegRightIndexOpen <= leftIndexClosed) return;

        int indentSpaceCount = (level << 1);
        if (debug && debugRangeUpdate) printf("%*sRangeUpdate, setting [%d, %d):%d into [%d, %d):%d.\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, newMinInRange, leftIndexClosed, rightIndexOpen, minInRange);

        if (newSegLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= newSegRightIndexOpen) {
            // Proactively stops at "full cover" update.
            minInRange = newMinInRange;
        } else {
            minInRange = INT_MAX;
            getOrCreateLChild()->RangeUpdate(newSegLeftIndexClosed, newSegRightIndexOpen, newMinInRange, level+1);    
            getOrCreateRChild()->RangeUpdate(newSegLeftIndexClosed, newSegRightIndexOpen, newMinInRange, level+1);
            minInRange = min(getOrCreateLChild()->minInRange, getOrCreateRChild()->minInRange);
        }

        if (debug && debugRangeUpdate) printf("%*sRangeUpdate, [%d, %d) updated to (minInRange:%d).\n", indentSpaceCount, "", leftIndexClosed, rightIndexOpen, minInRange);
    }

    int RangeMin(int targetLeftIndexClosed, int targetRightIndexOpen, int level) {
        // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
        if (targetLeftIndexClosed >= targetRightIndexOpen) return INT_MAX;
        if (targetLeftIndexClosed >= rightIndexOpen) return INT_MAX;
        if (targetRightIndexOpen <= leftIndexClosed) return INT_MAX;
    
        int indentSpaceCount = (level << 1);
    
        if (targetLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= targetRightIndexOpen) {
            if (debug && debugRangeMin) printf("%*sRangeMin1, [%d, %d) on node:[leftIndexClosed:%d, rightIndexOpen:%d) returning %d.\n", indentSpaceCount, "", targetLeftIndexClosed, targetRightIndexOpen, leftIndexClosed, rightIndexOpen, minInRange);
            return minInRange;
        }

        int toRet = INT_MAX;
        if (NULL != lChild) {
            int cand = lChild->RangeMin(targetLeftIndexClosed, targetRightIndexOpen, level+1); 
            toRet = min(toRet, cand);
        }

        if (NULL != rChild) {
            int cand = rChild->RangeMin(targetLeftIndexClosed, targetRightIndexOpen, level+1);
            toRet = min(toRet, cand);
        }

        if (INT_MAX == toRet) {
            // no child, thus all children intervals share the same "this->minInRange"
            toRet = minInRange;
        }

        if (debug && debugRangeMin) printf("%*sRangeMin2, [%d, %d) on node:[leftIndexClosed:%d, rightIndexOpen:%d) returning %d.\n", indentSpaceCount, "", targetLeftIndexClosed, targetRightIndexOpen, leftIndexClosed, rightIndexOpen, toRet);
        return toRet;
    }

    int firstServerAvailable(int targetLeftIndexClosed, int targetRightIndexOpen, int arrivalTime) {
        // binary search 
        int foundIndex = INVALID;
        int l = targetLeftIndexClosed, r = targetRightIndexOpen; 
        while (l < r) {
            int t = ((l + r) >> 1);
            int cand2 = RangeMin(targetLeftIndexClosed, t+1, 0);
            if (cand2 > arrivalTime) {
                l = t+1;
                continue;
            }
            // cand2 < arrivalTime, thus "t" is a valid server index 
            foundIndex = t;
            int cand1 = RangeMin(targetLeftIndexClosed, t, 0);
            if (debug) printf("firstServerAvailable(%d, %d, arrivalTime: %d), got t= %d with cand1= %d, cand2= %d\n", targetLeftIndexClosed, targetRightIndexOpen, arrivalTime, t, cand1, cand2);
            if (cand1 > arrivalTime) {
                break; // found the exact match
            }
            // cand1 < arrivalTime too, better option might exist
            r = t;
        }

        return foundIndex;
    }
};

/*
test cases
3
[1,2,3,4,5]
[5,2,3,3,3]
3
[1,2,3,4]
[1,2,1,2]
3
[1,2,3]
[10,12,11]
4
[1,3,4,5,10,12]
[11,9,3,1,9,12]
*/
class Solution {
public:
    vector<int> busiestServers(int k, vector<int>& arrival, vector<int>& load) {
        vector<int> ret;
        unordered_map<int, int> counter;
        SegTreeNode* root = new SegTreeNode(0, k);
        for (int i = 0; i < arrival.size(); ++i) {
            int arrivalTime = arrival[i], endTime = arrival[i]+load[i];
            int foundIndex = INVALID;
            int st = i%k;
            if (debug) printf("Seeking for %d-th arrival [arrivalTime: %d, endTime:%d, st:%d]\n", i, arrivalTime, endTime, st);
            foundIndex = root->firstServerAvailable(st, k, arrivalTime);
            if (INVALID == foundIndex) {
                foundIndex = root->firstServerAvailable(0, st, arrivalTime);
            }

            if (INVALID != foundIndex) {
                if (debug) printf("For the %d-th arrival [arrivalTime: %d, endTime:%d], foundIndex = %d\n", i, arrivalTime, endTime, foundIndex);
                ++counter[foundIndex];
                root->RangeUpdate(foundIndex, foundIndex+1, endTime, 0);
            }
        }

        int maxCnt = INT_MIN;
        for (auto &[k,v] : counter) maxCnt = max(maxCnt, v);
        for (auto &[k,v] : counter) if (v == maxCnt) ret.push_back(k);
        sort(ret.begin(), ret.end());
        // TODO delete root and write a deconstructor for SegTreeNode

        return ret;
    }
};
