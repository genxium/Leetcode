bool const debug = false;
/*
test cases
[1,-2,3,-2]
[5,-3,5]
[-3,-2,-3]
[-4,5,-3,5,1]
*/
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        vector<int> ps = {0}, psmin = {0}, psmax = {INT_MIN/2}, ss = {0}, ssmax = {INT_MIN/2}; // prefixSum & maxByFar, suffixSum & maxByFar
        int ans = INT_MIN;
        for (int i = 0; i < nums.size(); ++i) {
            ps.push_back(ps.back() + nums[i]);
            psmax.push_back(max(psmax.back(), ps.back()));
            if (debug) printf("#0, ps.back():%d, psmin.back():%d, ans updated to %d\n", ps.back(), psmin.back(), ans);
            ans = max(ans, ps.back()-psmin.back());
            if (debug) printf("#1, ps.back():%d, psmin.back():%d, ans updated to %d\n", ps.back(), psmin.back(), ans);
            psmin.push_back(min(psmin.back(), ps.back()));
        }
        
        for (int i = nums.size()-1; i >= 0; --i) {
            ss.push_back(ss.back() + nums[i]);
            ssmax.push_back(max(ssmax.back(), ss.back()));
        }
        
        for (int i = 0; i < nums.size(); ++i) {
            int j = nums.size()-i;
            int cand = psmax[i] + ssmax[j];
            ans = max(ans, cand);
            if (debug) printf("#2, psmax[i:%d]:%d, psmin[i:%d]:%d, ans updated to %d\n", i, psmax[i], i, psmin[i], ans);
        }
        
        return ans;
    }
};
