class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
      /*
      test case #1 
      [8,8,7,7,7]
      
      test case #2
      [0,-1,2,-1]
      
      test case #3
      [1,2,2,3,2,1,1,3]
      */
      
      /*
      It's obvious that there're at most 2 such elements, namely "first" and "second".
      */
      int n = nums.size();
      int countRequired = n/3;
    
      int first = INT_MAX, second = INT_MAX, firstCount = 0, secondCount = 0;
      vector<int> ans;
      
      /*
      First pass, locate 2 "MajorityElementCandidate"s.
      */
      for (int i = 0; i < n; ++i) {
        /*
        It's CRITICAL to check "first == nums[i]" and "second == nums[i]" prior to doing the zero-checks. 
        */
        if (0 < firstCount && first == nums[i]) {
          ++firstCount; 
          printf("Updated firstCount == %d(first:%d) @i:%d\n", firstCount, first, i);
        } else if (0 < secondCount && second == nums[i]) {
          ++secondCount;
          printf("Updated secondCount == %d(second:%d) @i:%d\n", secondCount, second, i);
        } else if (0 == firstCount) {
          first = nums[i];
          firstCount = 1;
          printf("Updated first == %d @i:%d\n", first, i);
        } else if (0 == secondCount) {
          second = nums[i];
          secondCount = 1;
          printf("Updated second == %d @i:%d\n", second, i);
        } else {
          --firstCount;
          --secondCount;
          printf("Decremented firstCount == %d(first:%d), secondCount == %d(second:%d) @i:%d\n", firstCount, first, secondCount, second, i);

        }
      }
      
      /*
      Second pass, validate that the 2 candidates "first" & "second" are 
      */
      firstCount = 0;
      secondCount = 0;
      for (auto &num : nums) {
        if (num == first) {
          ++firstCount;
        }
        if (num == second) {
          ++secondCount;
        }
      }
      
      if (firstCount > countRequired) {
        ans.push_back(first);
      }
      if (second != first && secondCount > countRequired) {
        ans.push_back(second);
      }
      
      return ans;
    }
};
