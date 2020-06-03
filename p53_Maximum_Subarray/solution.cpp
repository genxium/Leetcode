#include <stdio.h>
#include <vector>
#include <limits.h>
using namespace std;

class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int dp = nums[0]; // It's actually dp[0] = nums[0].
        // dp[i] = max subarray sum that ends exactly at nums[i]
        // dp[i] = (dp[i-1] > 0 ? (dp[i-1] + nums[i]) : nums[i])
        int arrLen = nums.size();
        int result = dp;
        for (int i = 1; i < arrLen; ++i) {
            int prevDp = dp;
            if (prevDp > 0) {
                dp = prevDp + nums[i];
            } else {
                dp = nums[i];
            }
            if (dp > result) {
                result = dp;
            }
        }
        return result;
    }
};

int main() {
  int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  vector<int> nums(arr, arr+sizeof(arr)/sizeof(int));
  Solution s;
  int res = s.maxSubArray(nums);
  printf("%d\n", res);
  return 0;
}
