#define MARKED -1

class Solution {
public:
    int missingNumber(vector<int>& nums) {
      int n = nums.size();
      for (int i = 0; i < n; ++i) {
        if (nums[i] == MARKED) continue;
        int j = nums[i];
        while (0 <= j && j < n && nums[j] != MARKED) {
          // printf("swapping nums[i:%d]:%d and nums[j:%d]:%d\n", i, nums[i], j, nums[j]);
          swap(nums[i], nums[j]);
          nums[j] = MARKED;
          // printf("set nums[j:%d] to MARKED\n", j);
          j = nums[i];
        }
      }
      
      for (int i = 0; i < n; ++i) {
        if (nums[i] == MARKED) continue;
        return i;
      }
      
      return n;
    }
};
