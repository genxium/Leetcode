#define StateTuple pair<int, int> // {groupIndex, groupElementStartIndexInArray}, here "group" is an alias to "subarr"
#define MAXN 1000
#define MAXM 50
#define INVALID -1
int cache[MAXM+1][MAXN+1]; // "cache[groupIndex][groupElementStartIndexInArray] == x" means that using array[groupElementStartIndexInArray...] to form the rest "m-groupIndex" groups results in "minimum largest group sum == x" for those rest groups
int prefixSums[MAXN+1];

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
*/
class Solution {
public:
    int solve(StateTuple s, int m, int n, vector<int> &nums) {
      int groupIndex = s.first, groupElementStartIndexInArray = s.second;
      if (groupIndex == m-1) {
        // we've no other grouping choice
        int currentGroupSum = (prefixSums[n-1] - (groupElementStartIndexInArray > 0 ? prefixSums[groupElementStartIndexInArray-1]:0));
        return (cache[groupIndex][groupElementStartIndexInArray] = currentGroupSum);
      }
      
      if (m-groupIndex > n-groupElementStartIndexInArray) {
        // insufficient rest elements for the grouping
        return INVALID;
      }
      
      if (groupElementStartIndexInArray == n) {
        // reaching end of "nums" yet "groupIndex < m-1"
        return INVALID;
      }
    
      if (INVALID != cache[groupIndex][groupElementStartIndexInArray]) {
        return cache[groupIndex][groupElementStartIndexInArray];
      }
      
      /*
      The recursive idea used here can be better understood by considering only the last 2 groups.
      ```
      (m-2)th                                   | (m-1)th a.k.a. last group
      ----------------------------------------------------------------------
      nums[groupElementStartIndexInArray,...,i1] | nums[i1+1, ...]
      nums[groupElementStartIndexInArray,...,i2] | nums[i2+1, ...]
      nums[groupElementStartIndexInArray,...,i3] | nums[i3+1, ...]
      ----------------------------------------------------------------------
      ```
      When calling solve({m-2, groupElementStartIndexInArray}), we iterate "i" over [i1, i2, i3] and might choose any group among the 6, i.e.
      - nums[groupElementStartIndexInArray,...,i1]
      - nums[i1+1, ...]
      - nums[groupElementStartIndexInArray,...,i2]
      - nums[i2+1, ...]
      - nums[groupElementStartIndexInArray,...,i3]
      - nums[i3+1, ...]
      as the value for "cache[groupIndex][groupElementStartIndexInArray]".
      */
      int currentGroupCacheHolder = INT_MAX;
      for (int i = groupElementStartIndexInArray; i < n; ++i) {
        int currentGroupSum = (prefixSums[i] - (groupElementStartIndexInArray > 0 ? prefixSums[groupElementStartIndexInArray-1]:0));
        int minOfLargestGroupSumOfTheRest = solve({groupIndex+1, i+1}, m, n, nums);
        if (INVALID == minOfLargestGroupSumOfTheRest) {
          // No valid grouping.
          continue;
        }
        int largestForThisI = max(currentGroupSum, minOfLargestGroupSumOfTheRest);
        currentGroupCacheHolder = min(currentGroupCacheHolder, largestForThisI);
      }
      
      return (cache[groupIndex][groupElementStartIndexInArray] = currentGroupCacheHolder);
    }
  
    int splitArray(vector<int>& nums, int m) {
      memset(cache, INVALID, sizeof(cache));
      memset(prefixSums, 0, sizeof(prefixSums));
      int ps = 0;
      int n = nums.size();
      for (int i = 0; i < n; ++i) {
        prefixSums[i] = ps + nums[i];
        ps += nums[i];
      }
      
      int ans = solve({0, 0}, m, n, nums);
      return ans;
    }
};
