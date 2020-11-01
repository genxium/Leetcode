// From https://github.com/genxium/Leetcode/blob/master/p462_Minimum_Moves_to_Equal_Array_Elements_II/submission_monotonicity.cpp, we know that the best "s" must be the median of nums. 

/*
test case#1
[0,0,6]

test case#2
[0,0,0,0,0,0,100,101,100,100,100,102,6,6,6,6,6,6,7,7,7,8]

test case#3
[203125577,-349566234,230332704,48321315,66379082,386516853,50986744,-250908656,-425653504,-212123143]
*/
typedef int64_t LL;

class Solution {
public:
    int minMoves2(vector<int>& nums) {
      int n = nums.size();
      if (0 == n) return 0;
      
      int ans = 0;
      if (n % 2 == 1) {
        int medianPos = n/2;
        nth_element (nums.begin(), nums.begin()+medianPos, nums.end());
        int s = nums[medianPos];
        for (auto num : nums) {
          ans += abs(num-s);
        }
      } else {
        int medianPos1 = (n-1)/2, medianPos2 = n/2;
        nth_element (nums.begin(), nums.begin()+medianPos1, nums.end());
        int s1 = nums[medianPos1];
        nth_element (nums.begin(), nums.begin()+medianPos2, nums.end());
        int s2 = nums[medianPos2];
        int candidate1 = 0, candidate2 = 0;
        for (auto num : nums) {
          candidate1 += abs(num-s1);
          candidate2 += abs(num-s2);
        }
        ans = min(candidate1, candidate2);
      }
      
      return ans;
    }
};
