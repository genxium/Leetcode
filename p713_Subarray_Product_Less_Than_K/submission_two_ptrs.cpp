bool debug = false;

/*
test cases
[10,5,2,6]
100
[10,9,10,4,3,8,3,3,6,2,10,10,9,3]
19
[1,1,1]
1
*/
class Solution {
public:
    int numSubarrayProductLessThanK(vector<int>& nums, int k) {
        if (0 == k) return 0;
        int n = nums.size();
        if (0 == n) return 0;
        
        int l = 0, r = 0;
        int prod = 1;
        int ans = 0;
        while (r < n) {
            prod *= nums[r];
            while (l < r && prod >= k) {
                prod /= nums[l];
                ++l;
            }
            
            if (l <= r && prod < k) {
                ans += (r-l+1);
            }
            ++r;
        }
        
        return ans;
    }
};
