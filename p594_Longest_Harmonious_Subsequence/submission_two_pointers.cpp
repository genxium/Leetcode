bool const debug = false;

/*
test cases
[1,3,2,2,5,2,3,7]
[1,2,3,4]
[1,1,1,1]
[1,2,3,3,1,-14,13,4]
[1]
[1,2]
[1,2,2,1]
*/
class Solution {
public:
    int findLHS(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int ans = 0, n = nums.size();
        int l = 0, r = 0;
        
        // loop 
        while (r < n) {
            while (l < r && nums[l] < nums[r] && nums[l] != nums[r]-1) ++l;
            if (r > l && nums[l] == nums[r]-1) {
                ans = max(ans, r-l+1);                
            }
            ++r;
        }
        
        if (debug) {
            printf("Settling, l:%d, r:%d\n", l, r);
        }
        
        return ans;
    }
};
