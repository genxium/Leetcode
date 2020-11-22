class Solution {
public:
    int waysToMakeFair(vector<int>& nums) {
      vector<int> psOdd = {0}, psEven = {0}; // prefixSum
      int n = nums.size();
      for (int i = 0; i < n; ++i) {
        if (i%2 == 0) {
          psEven.push_back(nums[i] + psEven.back());
        } else {
          psOdd.push_back(nums[i] + psOdd.back());
        }
      }
      
      int ans = 0;
      for (int i = 0; i < n; ++i) {
        // if "nums[i]" is removed
        if (0 == i) {
          int evenFirst = 0;
          int oddFirst = 0;
          int oddSecond = (psEven.back()-nums[0]);
          int evenSecond = (psOdd.back());
          //printf("i:%d, evenFirst: %d, evenSecond: %d, oddFirst: %d, oddSecond: %d\n", i, evenFirst, evenSecond, oddFirst, oddSecond);
          if (evenFirst + evenSecond == oddFirst + oddSecond) ++ans;
          continue;
        }
        
        if (i%2 == 0) {
          int iPsEven = (i/2+1);
          int evenFirst = (psEven[iPsEven]-nums[i]);
          int oddSecond = (psEven.back()-psEven[iPsEven]);
          
          int iPsOdd = ((i-1)/2+1);
          int oddFirst = (psOdd[iPsOdd]);
          int evenSecond = (psOdd.back()-psOdd[iPsOdd]);
          //printf("i:%d, iPsEven:%d, evenFirst: %d, evenSecond: %d, iPsOdd:%d, oddFirst: %d, oddSecond: %d\n", i, iPsEven, evenFirst, evenSecond, iPsOdd, oddFirst, oddSecond);
          if (evenFirst + evenSecond == oddFirst + oddSecond) ++ans;
        } else {
          int iPsOdd = (i/2+1);
          int oddFirst = (psOdd[iPsOdd]-nums[i]);
          int evenSecond = (psOdd.back()-psOdd[iPsOdd]);
          
          int iPsEven = ((i-1)/2+1);
          int evenFirst = (psEven[iPsEven]);
          int oddSecond = (psEven.back()-psEven[iPsEven]);
          //printf("i:%d, iPsEven:%d, evenFirst: %d, evenSecond: %d, iPsOdd:%d, oddFirst: %d, oddSecond: %d\n", i, iPsEven, evenFirst, evenSecond, iPsOdd, oddFirst, oddSecond);
          if (evenFirst + evenSecond == oddFirst + oddSecond) ++ans;
        }
      }
      return ans;
    }
};
