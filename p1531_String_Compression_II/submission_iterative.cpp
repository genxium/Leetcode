#define MAXN 101
#define MAX_LAST_CH_CONSECUTIVE_COUNT 10
#define MAXQUOTA 101
#define MAXCH 27
#define INVALID -1
#define INVALID_CH 'a'+26

/*
"dp[i][delCount] == x" means that deleting EXACTLY "delCount" characters in "s[0, ..., i]" can result in a minimum run-length "x"

dp[i][delCount] = min{
                      1 + runLengthOf(i-j-f(j+1, i)) + dp[j][delCount-f(j+1, i)] | j < i && delCount-f(j+1, i) >= 0 && f(j+1, i) is the "quota allowed and necessary deletion to make s[j+1, ..., i] contain all same characters as `s[i]` whilst different from `s[j]`"
                      ,
                      dp[i-1][delCount-1]
                      }
*/
int dp[MAXN][MAXQUOTA];

class Solution {
private:
    int runLengthOf(int count) {
      if (count <= 1) {
        return 0;
      }
      if (count < 10) {
        return 1;
      }
      if (count < 100) {
        return 2;
      }
      return 3;
    }
public:
    int getLengthOfOptimalCompression(string s, int k) {
      /*
      test case#1
      "aabbaa"
      2
      
      test case #2
      "bbcbbbaccc"
      1
      
      test case #3
      "aabaabbcbbbaccc"
      1
      
      test case #4
      "aabaabbcbbbaccc"
      6
      
      test case #5
      "aaaaaaaaaaa"
      0
      */
      // init
      int n = s.length();
      for (int i = 0; i < n; ++i) {
        int delCountUpper = min(i+1, k);
        for (int delCount = 0; delCount <= delCountUpper; ++delCount) {
          dp[i][delCount] = INT_MAX;
        }
      }
      dp[0][0] = 1;
      dp[0][1] = 0;
      
      for (int i = 1; i < n; ++i) {
        int delCountUpper = min(i+1, k);
        for (int delCount = 0; delCount <= delCountUpper; ++delCount) {
          //printf("computing dp[i:%d][delCount:%d]\n", i, delCount);
          // deleting s[i]
          if (delCount - 1 >= 0) {
            dp[i][delCount] = dp[i-1][delCount-1];            
          }
            
          char lastCh = s[i];
          
          // keeping s[i]
          int f = 0, j = i-1;
          for(; j >= 0; --j) {
            if (s[j] != lastCh) {
              //printf("\t@j:%d, checking milestone by f:%d\n", j, f);
              int countOfConsecutiveLastCh = i-j-f; // here "f" represents f(j+1, i)
              int countRunLength = runLengthOf(countOfConsecutiveLastCh);
              dp[i][delCount] = min(dp[i][delCount], 1+countRunLength+dp[j][delCount-f]);
              ++f;
              if (delCount < f) break;
            }
          }
          if (0 > j) {
            // wrap up
            //printf("\t@j:%d, checking milestone by f:%d\n", j, f);
            int countOfConsecutiveLastCh = i-j-f; // here "f" represents f(j+1, i)
            int countRunLength = runLengthOf(countOfConsecutiveLastCh);
            dp[i][delCount] = min(dp[i][delCount], 1+countRunLength);  
          }
        }
      }
      
      return dp[n-1][k];
    }
};
