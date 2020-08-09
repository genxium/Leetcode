#define ULL int64_t
#define MAXN 100000
#define MOD 1000000007u
#define INVALID -1
ULL dp1[MAXN];
ULL dp2[MAXN];

class Solution {
public:
    ULL add(ULL a, int b) {
      ULL ret = (ULL)a;
      ret += (ULL)b;
      return ret;
    }
  
    int maxSum(vector<int>& nums1, vector<int>& nums2) {
      /*
      test case #1
      [2,4,5,8,10]
      [4,6,8,9]

      test case #2
      [1,4,5,6,12,14]
      [10,13,16,21,27]
      */
      memset(dp1, 0u, sizeof(dp1));
      memset(dp2, 0u, sizeof(dp2));
      int iUpper = nums1.size(), jUpper = nums2.size();
      int i = 0, j = 0;
      // init
      dp1[i] = (ULL)nums1[i];
      dp2[j] = (ULL)nums2[j];
      
      // loop
      while (i < iUpper && j < jUpper) {
        if (nums1[i] > nums2[j]) {
          auto it = lower_bound(nums2.begin(), nums2.end(), nums1[i]);
          int anotherJ = (int)(it - nums2.begin());
          //printf("anotherJ == %d\n", anotherJ, jUpper);
          ++j;
          while (j < jUpper && j < anotherJ) {
            dp2[j] = add(dp2[j-1], nums2[j]);
            //printf("\tdp2[j:%d] == %d, from\n\t\tdp2[j-1:%d]:%d+nums2[j:%d]:%d\n", j, dp2[j], j-1, dp2[j-1], j, nums2[j]);
            if (nums2[j] == nums1[i]) break;
            ++j;
          }
          //printf("\tnow that j == %d\n", j);
          if (j < jUpper) {
            dp2[j] = add(dp2[j-1], nums2[j]);
            //printf("\tdp2[j:%d] == %d, from\n\t\tdp2[j-1:%d]:%d+nums2[j:%d]:%d\n", j, dp2[j], j-1, dp2[j-1], j, nums2[j]);
          }
        } else if (nums1[i] < nums2[j]) {
          auto it = lower_bound(nums1.begin(), nums1.end(), nums2[j]);
          int anotherI = (int)(it - nums1.begin());
          //printf("anotherI == %d\n", anotherI, iUpper);
          ++i;
          while (i < iUpper && i < anotherI) {
            dp1[i] = add(dp1[i-1], nums1[i]);
            //printf("\tdp1[i:%d] == %d, from\n\t\tdp1[i-1:%d]:%d+nums1[i:%d]:%d\n", i, dp1[i], i-1, dp1[i-1], j, nums1[i]);
            ++i;
          }
          //printf("\tnow that i == %d\n", i);
          if (i < iUpper) {
            dp1[i] = add(dp1[i-1], nums1[i]);
            //printf("\tdp1[i:%d] == %d, from\n\t\tdp1[i-1:%d]:%d+nums1[i:%d]:%d\n", i, dp1[i], i-1, dp1[i-1], j, nums1[i]);
          }
        } else {
          // nums1[i] == nums2[j]
          //printf("nums1[i:%d] == nums2[j:%d]\n", i, j);
          ULL old1 = dp1[i], old2 = dp2[j]; 
          dp1[i] = max(old2, add((i > 0 ? dp1[i-1] : 0u), nums1[i]));
          //printf("\tdp1[i:%d] == %d\n", i, dp1[i]);

          dp2[j] = max(old1, add((j > 0 ? dp2[j-1] : 0u), nums2[j]));
          //printf("\tdp2[j:%d] == %d\n", j, dp2[j]);

          ++i;
          if (i < iUpper) {
            dp1[i] = add(dp1[i-1], nums1[i]);
            //printf("\tdp1[i:%d] == %d, from\n\t\tdp1[i-1:%d]:%d+nums1[i:%d]:%d\n", i, dp1[i], i-1, dp1[i-1], j, nums1[i]);  
          }
          
          ++j;
          if (j < jUpper) {
            dp2[j] = add(dp2[j-1], nums2[j]);
            //printf("\tdp2[j:%d] == %d, from\n\t\tdp2[j-1:%d]:%d+nums2[j:%d]:%d\n", j, dp2[j], j-1, dp2[j-1], j, nums2[j]); 
          }
        }
      }
      
      // wrap up
      while (j < jUpper) {
        dp2[j] = add((j > 0 ? dp2[j-1] : 0u), nums2[j]);
        ++j;
      }
      
      while (i < iUpper) {
        dp1[i] = add((i > 0 ? dp1[i-1] : 0u), nums1[i]);
        ++i;
      }
      
      ULL ans = max(dp1[iUpper-1], dp2[jUpper-1]);
      return (int)(ans%MOD);
    }
};
