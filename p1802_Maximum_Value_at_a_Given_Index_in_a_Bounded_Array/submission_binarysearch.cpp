bool debug = false;
int const INVALID = -1;
typedef int64_t LL;

/*
test cases
4
2
6
6
1
10
8
7
14
*/
class Solution {
public:
    int maxValue(int n, int index, int maxSum) {
        LL l = 0, r = 1000000001;
        LL maxSumL = (LL)maxSum;
        int leftCnt = index, rightCnt = n-index; // the right part includes "index" itself

        int ans = INVALID;
        while (l < r) {
            int val = (l+r)/2;
            LL leftEdgeV = val-1 - (index-1); // nums[index-1] + (nums[index-1]-1) + ... + (nums[index-1]-(index-1)), totally "leftCnt" elements, where "nums[index-1]" is assumed to be "val-1"
            LL rightEdgeV = val - (rightCnt-1); // nums[index] + (nums[index] - 1) + ... + (nums[index] - (rightCnt-1)), totally "rightCnt" elements
            LL leftSumPatch = 0;
            LL rightSumPatch = 0;
            if (leftEdgeV <= 0) {
                // We want to "make this val possible", i.e. sum to not greater than "maxSum", thus on the left side decrease it to 1 asap, then patch by 1. Thus there're [leftEdgeV, ..., 0] elements all incremented to 1.
                int toPatchCnt = 0-leftEdgeV+1;
                leftSumPatch = toPatchCnt*1 - (0+leftEdgeV)*toPatchCnt/2;
            }
            if (rightEdgeV <= 0) {
                // Similarly
                int toPatchCnt = 0-rightEdgeV+1;
                rightSumPatch = toPatchCnt*1 - (0+rightEdgeV)*toPatchCnt/2;
            }
            
            LL leftSum = (LL)leftCnt*(leftEdgeV + val-1)/2 ; 
            LL rightSum = (LL)rightCnt*(val + rightEdgeV)/2; 
            LL cand = leftSum+rightSum+leftSumPatch+rightSumPatch;
            if (debug) printf("Checking Val == %ld, leftEdgeV:%ld, rightEdgeV:%ld, leftSum:%ld, rightSum:%ld, leftSumPatch:%ld, rightSumPatch:%ld, cand:%ld, now l:%ldd, r:%ld,\t", val, leftEdgeV, rightEdgeV, leftSum, rightSum, leftSumPatch, rightSumPatch, cand, maxSumL, l, r);
            if (cand > maxSumL) {
                if (debug) printf("BAD\n");
                r = val;
            } else {
                if (debug) printf("OK\n");
                ans = max(ans, val);
                l = val+1;
            }
        }

        return ans;
    }
};
