#define MAXN 1000
#define BIGINT int64_t
#define PI pair<int, BIGINT> // {length, diff}
#define PIM map<PI, int> 

/*
test case#1
[2,4,6,8,10]

test case#2
[2,4,6,8,11]

test case#3
[2,2,3,4]

test case#4
[0,2000000000,-294967296]
*/
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
      int n = A.size(), ans = 0;
      vector<PIM> dp; // "dp[length][endIdxClosed][diff] == x" means that count of ArithmeticSubsequence"s that end exactly at "endIdxClosed" with "length" is "x"

      // Init
      for (int length = 0; length <= n; ++length) {
        PIM singleDict;
        dp.push_back(singleDict);
      }
      
      for (int endIdxClosed = 0; endIdxClosed < n; ++endIdxClosed) {
        dp[1][{endIdxClosed,0}] = 1; // For each "1-length ArithmeticSubsequence" that ends at each "A[i]", we have only 1 count and "diff == 0".  
      }

      // Loop
      for (int length = 2; length <= n; ++length) {
        int preLength = length-1;
        auto &preDict = dp[preLength];
        int endIdxClosedLower = (length-1); 
        for (int endIdxClosed = endIdxClosedLower; endIdxClosed < n; ++endIdxClosed) {
          for (auto it = preDict.begin(); it != preDict.end(); ++it) {
            auto prevKey = it->first;
            int prevEndIdxClosed = prevKey.first;
            if (prevEndIdxClosed >= endIdxClosed) continue;
            
            BIGINT prevDiff = prevKey.second;
            int prevCount = it->second;
            BIGINT diff = (BIGINT)A[endIdxClosed]-(BIGINT)A[prevEndIdxClosed];
            if (preLength != 1 && prevDiff != diff) continue;
            
            if (length >= 3) {
              //printf("@endIdxClosed:%d, adding prevCount==%d from dp[preLength:%d][{prevEndIdxClosed:%d, prevDiff:%d}]\n", endIdxClosed, prevCount, preLength, prevEndIdxClosed, prevDiff);
              ans += prevCount;
            }
            auto &curDict = dp[length];
            PI key = {endIdxClosed, diff};
            if (curDict.find(key) == curDict.end()) {
              curDict[key] = prevCount;
            } else {
              curDict[key] += prevCount;  
            }
          }
        }
      }
      
      return ans;
    }
};
