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
      
      vector<pair<int, int>> sortedValsWithCounts;
      sort(nums.begin(), nums.end());
      
      sortedValsWithCounts.push_back({nums[0], 1});
      for (int i = 1; i < n; ++i) {
        auto &last = sortedValsWithCounts.back();
        if (nums[i] > last.first) {
          sortedValsWithCounts.push_back({nums[i], 1});
        } else {
          ++last.second;
        }
      }
      
      int m = sortedValsWithCounts.size();
      vector<int> countPrefixSums;
      countPrefixSums.push_back(sortedValsWithCounts[0].second);
      for (int i = 1; i < m; ++i) {
        int last = countPrefixSums.back();
        countPrefixSums.push_back(sortedValsWithCounts[i].second + last);
      }
      
      // init
      int s = sortedValsWithCounts[0].first; // let's start with nums[0], and calculate SUM(|sortedValsWithCounts[i].first - s|*sortedValsWithCounts[i].second)
      
      int64_t currentTotalMoves = 0;
      for (int i = 0; i < m; ++i) {
        currentTotalMoves += abs(sortedValsWithCounts[i].first - s)*sortedValsWithCounts[i].second;
      }
      // printf("Initially currentTotalMoves == %d\n", currentTotalMoves);
      
      // loop
      for (int i = 1; i < m; ++i) {
        // by now "sortedValsWithCounts[i-1].first <= s < sortedValsWithCounts[i].first"
        int restCountPrefixSums = countPrefixSums[m-1]-countPrefixSums[i-1];
        int deltaTotalMoves = countPrefixSums[i-1]-restCountPrefixSums; // if s increments by 1 but still keeps "s < sortedValsWithCounts[i].first"
        // printf("@sortedValsWithCounts[i:%d] {val:%d, count:%d}, countPrefixSums[i-1] == %d, restCountPrefixSums == %d, \n", i, sortedValsWithCounts[i].first, sortedValsWithCounts[i].second, countPrefixSums[i-1], restCountPrefixSums);
        if (deltaTotalMoves < 0) {
          currentTotalMoves = currentTotalMoves + deltaTotalMoves*(sortedValsWithCounts[i].first-s);
        } else {
          // no need to move forward
          break;
        }
        
        // sets "s" 
        s = sortedValsWithCounts[i].first;
      }
      
      return currentTotalMoves;
    }
};
