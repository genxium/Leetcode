#define larger(x, y) x > y ? x : y
#define smaller(x, y) x < y ? x : y
/*
test case#1
[7,2,5,10,8]
2

test case#2
[1,2,3,4,5]
2

test case#3
[1,4,4]
3

test case#4
[7,2,5,10,8]
3

test case#5
[7,2,103,0,5,10,0,8,999999,10]
3

test case#6
[7,2,103,0,5,10,0,8,99,10]
5

test case#7
[2,3,1,2,4,3]
5
*/
class Solution {
public:
    bool isAvailable(int n, int m, vector<int> &nums, int candidateMinOfLargestGroupSum) {
      int lastGroupSum = 0;
      for (int i = 0; i < n; ++i) {
        if (lastGroupSum > candidateMinOfLargestGroupSum) {
          return false;
        } 
        int tmp = lastGroupSum + nums[i];
        if (tmp > candidateMinOfLargestGroupSum) {
          // just beyond the border
          // reset "candidateMinOfLargestGroupSum" for the next group
          lastGroupSum = nums[i];
          --m;
        } else {
          lastGroupSum = tmp;
        }
      }
  
      // wrap up
      if (lastGroupSum <= candidateMinOfLargestGroupSum) {
        --m;
      } else {
        return false;
      }
      
      //printf("isAvailable(candidateMinOfLargestGroupSum: %d), remaining m:%d, lastGroupSum:%d\n", candidateMinOfLargestGroupSum, m, lastGroupSum);
  
      if (0 <= m) {
        // In this case we can always cut the current grouping into even smaller groups.
        return true;
      }
      return false;
    }

    int splitArray(vector<int>& nums, int m) {
      int l = 0, r = 0;
      int n = nums.size();
      for (int i = 0; i < n; ++i) {
        r += nums[i];
      }
            
      int ans = INT_MAX;
      while (l <= r) {
        int candidateMinOfLargestGroupSum = ((l+r) >> 1);
        if (isAvailable(n, m, nums, candidateMinOfLargestGroupSum)) {
          ans = smaller(ans, candidateMinOfLargestGroupSum);
          r = candidateMinOfLargestGroupSum-1;
        } else {
          l = candidateMinOfLargestGroupSum+1;
        }
      }
      
      return ans;
    }
};

