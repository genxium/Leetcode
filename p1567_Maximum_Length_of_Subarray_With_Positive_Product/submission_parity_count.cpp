#define INVALID -1
/*
test case #1
[-1000000000,-1000000000]
*/
class Solution {
public:
    bool isOdd(int x) {
        return ((x & 1) > 0);
    }
    int search(vector<int> &nums, int st, int &ans) {
        int n = nums.size();
        int prefixNegativeCount = 0;
        int leftmostOddNegativePos = INVALID;
        int leftmostEvenNegativePos = st-1;
        int i;
        for (i = st; i < n; ++i) {
            if (0 == nums[i]) return i;
            if (0 > nums[i]) {
                ++prefixNegativeCount;
            }
            if (INVALID == leftmostOddNegativePos && isOdd(prefixNegativeCount)) {
                leftmostOddNegativePos = i;
            }
            if (!isOdd(prefixNegativeCount)) {
                ans = max(ans, i-leftmostEvenNegativePos);     
            } else {
                ans = max(ans, i-leftmostOddNegativePos);   
            }
        }
        return i;
    }
    
    int getMaxLen(vector<int>& nums) {
        int st = 0, n = nums.size();
        int ans = 0;
        while (st < n) {
            while (st < n && 0 == nums[st]) ++st;
            if (st >= n) break;
            int nextSt = search(nums, st, ans);    
            st = nextSt;
        }
        return ans;
    }
};
