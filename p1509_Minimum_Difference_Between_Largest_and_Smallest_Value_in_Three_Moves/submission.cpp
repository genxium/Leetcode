class Solution {
public:
    
    int minDifference(vector<int>& nums) {
      int n = nums.size();
      if (n <= 3) return 0;
      
      // now that we only consider n >= 4
      sort(nums.begin(), nums.end());
      int origMinCount = 0;
      int origMaxCount = 0;
      
      int origMin = nums[0];
      int origMax = nums[n-1]; 
      if (origMax == origMin) return 0;
      
      // now that "origMax > origMin"
      for (int i = 0; i < 3; ++i) {
        if (nums[i] != origMin) break;
        ++origMinCount;
      }
      
      for (int i = n-1; i >= n-3; i--) {
        if (nums[i] != origMax) break;
        ++origMaxCount;
      }
      
      //printf("origMin:%d, origMinCount:%d, origMax:%d, origMaxCount:%d\n", origMin, origMinCount, origMax, origMaxCount);

      // note that by now "origMaxCount >= 1" , "origMinCount >= 1", therefore "origMaxCount + origMinCount >= 2"
      if (origMaxCount > 3 &&  origMinCount > 3) {
        // we can do nothing
        return (origMax - origMin);
      } else {
        if (origMaxCount > 3) {
          // but origMinCount <= 3
          int minTarget = origMin;
          for (int i = 0; i < 4; ++i) {
            if (nums[i] > minTarget) {
              minTarget = nums[i];
            }
          }
          return (origMax - minTarget);
        }
        if (origMinCount > 3) {
          // but origMaxCount <= 3
          int maxTarget = origMax;
          for (int i = n-1; i >= n-4; --i) {
            if (nums[i] < maxTarget) {
              maxTarget = nums[i];
            }
          }
          return (maxTarget - origMin);
        }
        
        // now that "1 <= origMinCount <= 3 && 1<= origMaxCount <= 3"
        int ans = INT_MAX;
        {
          int minTarget = origMin;
          int maxTarget = origMax;
          // 3 0
          for (int i = 0; i < 4; ++i) {
            if (nums[i] > minTarget) {
              minTarget = nums[i];              
            }
          }
          ans = min(ans, maxTarget-minTarget);
        }
        {
          int minTarget = origMin;
          int maxTarget = origMax;
          // 2 1
          for (int i = 0; i < 3; ++i) {
            if (nums[i] > minTarget) {
              minTarget = nums[i];              
            }
          }
          for (int i = n-1; i >= n-2; --i) {
            if (nums[i] < maxTarget) {
              maxTarget = nums[i];
            }
          }
          ans = min(ans, maxTarget-minTarget);
        }
         {
          int minTarget = origMin;
          int maxTarget = origMax;
          // 1 2
          for (int i = 0; i < 2; ++i) {
            if (nums[i] > minTarget) {
              minTarget = nums[i];              
            }
          }
          for (int i = n-1; i >= n-3; --i) {
            if (nums[i] < maxTarget) {
              maxTarget = nums[i];
            }
          }
          ans = min(ans, maxTarget-minTarget);
        }
        {
          int minTarget = origMin;
          int maxTarget = origMax;
          // 0 3
          for (int i = n-1; i >= n-4; --i) {
            if (nums[i] < maxTarget) {
              maxTarget = nums[i];
            }
          }
          ans = min(ans, maxTarget-minTarget);
        }
        return ans; 
      }
    }
};
