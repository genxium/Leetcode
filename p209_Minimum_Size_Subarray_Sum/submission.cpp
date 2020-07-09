class Solution {
public:
    int minSubArrayLen(int s, vector<int>& nums) {
      /*
      test case #1
      11
      [1,2,3,4,5]
      
      test case #2
      3
      [1,1]
      */
      int n = nums.size();
      if (0 == n) return 0;
      int l = 0, r = 0, sum = 0, ans = INT_MAX;
      while (r < n) {
        while (sum < s) {
          sum += nums[r];
          ++r;
          if (r == n) break;
        }
        // now that "sum >= s" but "sum - nums[r-1] < s"
        //printf("l:%d, r:%d, sum:%d >= s:%d\n", l, r, sum, s);
        if (sum >= s && r - l < ans) {
          ans = (r - l);
        }
        
        while (l < r && sum >= s) {
          sum -= nums[l];
          ++l;
          if (sum >= s && r - l < ans) {
            ans = (r - l);
            //printf("l:%d, r:%d, sum:%d >= s:%d\n", l, r, sum, s);
          }
        }
        // now that "sum < s" but "sum + nums[l-1] >= s"
        //printf("l:%d, r:%d, sum:%d < s:%d\n", l, r, sum, s);
      }
      if (INT_MAX == ans) {
        return 0;
      }
      return ans;
    }
};
