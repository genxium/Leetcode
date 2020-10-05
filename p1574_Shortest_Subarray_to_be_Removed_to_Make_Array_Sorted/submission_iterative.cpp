#define PI pair<int, int>
#define INVALID -1

/*
test case#1
[5,4,3,2,1]

test case#2
[1,2,3,10,0,7,8,9]

test case#3
[16,10,0,3,22,1,14,7,1,12,15]
*/
class Solution {
public:
    int findLengthOfShortestSubarray(vector<int>& arr) {
      int n = arr.size();
      int lSt = 0, lEd = 1, rSt = n-1, rEd = n; // both [lSt, lEd) and [rSt, rEd) are non-decreasing subarrays, each starting from the arr[0] or ending at arr[n-1], respectively
      
      while (lEd < n && arr[lEd] >= arr[lEd-1]) ++lEd;
      if (lEd == n) return 0;
            
      while (rSt-1 >= 0 && arr[rSt-1] <= arr[rSt]) --rSt;
      
      //printf("lEd: %d, rSt: %d\n", lEd, rSt);

      int ans = n-(rEd-rSt); // init with the solution that only keeps [rSt, rEd).
        
      // find the optimal solution such that [lSt, lEd) and [rSt, rEd) concatenate as a monolithic non-decreasing array
      for (int leftJoint = 0; leftJoint < lEd; ++leftJoint) {
        int rightJoint = rSt;
        while (rightJoint < rEd && arr[rightJoint] < arr[leftJoint]) {
          ++rightJoint;
        }
        ans = min(ans, rightJoint-leftJoint-1);
      }
      return ans;
    }
};
