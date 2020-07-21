class Solution {
public:
    void moveZeroes(vector<int>& nums) {
      /*
      test case #1
      [0,1,0,7,9,9,0,0,0,2,4,0,7,3,12]
      
      */
      int n = nums.size();
      int cursor = 0;
      for (int i = 0; i < n; ++i) {
        if (nums[i] != 0) {
          nums[cursor++] = nums[i];
        } 
      }
      for (int i = cursor; i < n; ++i) {
        nums[i] = 0;
      }
    }
};
