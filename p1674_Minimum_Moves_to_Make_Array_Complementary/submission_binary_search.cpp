bool const debug = false;
/*
test cases
[1,2,4,3]
4
[1,2,2,1]
2
[1,2,1,2]
2
[28,50,76,80,64,30,32,84,53,8]
84
[34,24,35,17,58,40,51,28,15,63,36,3,26,30,61,38,45,63]
70
[16,34,41,55,41,32,13,43,25,34,57,8,32,25,55,48,57,19,47,54,58,15,49,52,45,32,10,26,19,53,58,37,3,32,21,11,7,7]
60
[20,17,76,41,29,31,21,57,49,56,12,63,5,54,44,81,53,55,40,27,18,71,51,41,1,46,67,2,79,66,51,74,3,38,65,25,45,9]
81
*/
class Solution {
public:
    int minMoves(vector<int>& nums, int limit) {
      int n = nums.size(), m = (n >> 1);
      vector<int> singleOpLowestVals, singleOpHighestVals, sums;
      unordered_map<int, int> counter;
      for (int i  = 0; i < m; ++i) {
        int a = nums[i], b = nums[n-1-i];
        int localSum = a + b;
        int singleOpLowestVal = localSum - max(a-1, b-1);
        singleOpLowestVals.push_back(singleOpLowestVal);
        int singleOpHighestVal = localSum + max(limit-a, limit-b);
        singleOpHighestVals.push_back(singleOpHighestVal);

        ++counter[localSum];
        sums.push_back(localSum);
      }

      sort(singleOpLowestVals.begin(), singleOpLowestVals.end());
      sort(singleOpHighestVals.begin(), singleOpHighestVals.end(), greater<int>());

      int ans = INT_MAX;
      for (auto s : sums) {
        // [it1, singleOpLowestVals.end()) are pairs that are "too big to reduce into `s` within 1 op"
        auto it1 = upper_bound(singleOpLowestVals.begin(), singleOpLowestVals.end(), s);
        int count1 = (singleOpLowestVals.end() - it1);
        
        // [it2, singleOpHighestVals.end()) are pairs that are "too small to raise into `s` within 1 op"
        auto it2 = upper_bound(singleOpHighestVals.begin(), singleOpHighestVals.end(), s, greater<int>());
        int count2 = (singleOpHighestVals.end() - it2);

        int countThatNeedsTwoOps = max(count1, count2); 
        int cand = countThatNeedsTwoOps*2 + m-countThatNeedsTwoOps-counter[s];
        ans = min(ans, cand);
      }
      return ans; 
    }
};
