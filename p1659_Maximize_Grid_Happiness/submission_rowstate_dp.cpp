int const INVALID = -1;
int const MAXY = 5;
int const MAXX = 5;
int const MAXINTROCOUNT = 6;
int const MAXEXTROCOUNT = 6;
int const INTROVERT = 1;
int const EXTROVERT = 2;
int const EMPTY = 0;
int const ROW_STATE_UPPER = (3*3*3*3*3); // Each cell has 3 states: introvert, extrovert, empty.
int introCountOf[ROW_STATE_UPPER], extroCountOf[ROW_STATE_UPPER];
int horizontalHappiness[ROW_STATE_UPPER], verticalHappiness[ROW_STATE_UPPER][ROW_STATE_UPPER];

int dp[MAXY][MAXINTROCOUNT+1][MAXEXTROCOUNT+1][ROW_STATE_UPPER]; // "dp[y][introCount][extroCount][rowState] == t" means that when the last row is in "rowState", using in total "introCount & extroCount", the "grid[0,...,y]" results in max happiness "t", btw "(3*3*3*3*3)*5*7*7 == 59535"

/*
test case#1
2
3
1
2

test case#2
3
1
2
1

test case#3
2
2
4
0

test case#4
5
5
1
2

test case#5
5
5
6
6
*/
class Solution {
public:
    string printRowState(int rowState, int n) {
      string toRet = "";
      while (n--) {
        int modDigit = rowState%3;
        toRet.push_back('0'+modDigit);
        rowState /= 3;
      }
      return toRet;
    }
  
    int countDigitInStateByBase(int state, int digit, int base) {
      if (0 == state) {
        if (0 == digit) return 1;
        else return 0;
      }
      int toRet = 0;
      while (state > 0) {
        int modDigit = state%base;
        if (modDigit == digit) ++toRet;
        state /= base;
      }
      return toRet;
    }
  
    int countIntroverts(int rowState) {
      return countDigitInStateByBase(rowState, INTROVERT, 3);
    }
  
    int countExtroverts(int rowState) {
      return countDigitInStateByBase(rowState, EXTROVERT, 3);
    }
  
    int countHorizontalHappiness(int rowState) {
      int toRet = 0, prevDigit = 0;
      while (rowState > 0) {
        int modDigit = rowState%3;
        if (modDigit == INTROVERT) {
          toRet += 120;
          if (prevDigit != EMPTY) {
            if (prevDigit == INTROVERT) {
              toRet -= 60;
            } else {
              toRet += 20;
              toRet -= 30;
            }
          }
        }
        
        if (modDigit == EXTROVERT) {
          toRet += 40;
          if (prevDigit != EMPTY) {
            if (prevDigit == EXTROVERT) {
              toRet += 40;
            } else {
              toRet += 20;
              toRet -= 30;
            }
          }
        }
        prevDigit = modDigit;
        rowState /= 3;
      }
      return toRet;
    }
  
    int countVerticalHappiness(int rowStateUp, int rowStateDown, int n) {
      int toRet = 0;
      while (n--) {
        int modDigitUp = rowStateUp%3;
        int modDigitDown = rowStateDown%3;
        if (modDigitUp == INTROVERT) {
          if (modDigitDown == INTROVERT) {
            toRet -= 60;
          }
          if (modDigitDown == EXTROVERT) {
            toRet += 20;
            toRet -= 30;
          }
        }
        if (modDigitUp == EXTROVERT) {
          if (modDigitDown == EXTROVERT) {
            toRet += 40;
          }
          if (modDigitDown == INTROVERT) {
            toRet += 20;
            toRet -= 30;
          }
        }
        rowStateUp /= 3;
        rowStateDown /= 3;
      }
      return toRet;
    }
  
    int getMaxGridHappiness(int m, int n, int introvertsCountLimit, int extrovertsCountLimit) {
      int ans = 0;
      if (0 == introvertsCountLimit && 0 == extrovertsCountLimit) return ans;
        
      // init
      memset(dp, INVALID, sizeof(dp));
      memset(introCountOf, 0, sizeof(introCountOf));
      memset(extroCountOf, 0, sizeof(extroCountOf));
      memset(horizontalHappiness, 0, sizeof(horizontalHappiness));
      memset(verticalHappiness, 0, sizeof(verticalHappiness));
      int rowStateLimit = 1;
      for (int i = 0; i < n; ++i) rowStateLimit *= 3;
      
      for (int rowState = 0; rowState < rowStateLimit; ++rowState) {
        int introCount = countIntroverts(rowState);
        int extroCount = countExtroverts(rowState);
        introCountOf[rowState] = introCount;
        extroCountOf[rowState] = extroCount;
        if (introCount > introvertsCountLimit) continue;
        if (extroCount > extrovertsCountLimit) continue;
        int t = countHorizontalHappiness(rowState);
        dp[0][introCount][extroCount][rowState] = t;
        ans = max(t, ans);
        //printf("dp[0][introCount:%d][extroCount:%d][rowState:%s] == %d\n", introCount, extroCount, printRowState(rowState, n).c_str(), t);
        
        horizontalHappiness[rowState] = t;
      }
      
      for (int rowStateUp = 0; rowStateUp < rowStateLimit; ++rowStateUp) {
        for (int rowStateDown = 0; rowStateDown < rowStateLimit; ++rowStateDown) {
          int u = countVerticalHappiness(rowStateUp, rowStateDown, n);
          verticalHappiness[rowStateUp][rowStateDown] = u;
        }  
      }
      
      // loop 
      for (int y = 0; y < m-1; ++y) {
        for (int rowStateUp = 0; rowStateUp < rowStateLimit; ++rowStateUp) {
          for (int introCount = 0; introCount <= introvertsCountLimit; ++introCount) {
            for (int extroCount = 0; extroCount <= extrovertsCountLimit; ++extroCount) {
              if (INVALID == dp[y][introCount][extroCount][rowStateUp]) continue;
              for (int rowStateDown = 0; rowStateDown < rowStateLimit; ++rowStateDown) {
                int introCountDown = introCountOf[rowStateDown];
                int extroCountDown = extroCountOf[rowStateDown];
                int newIntroCount = introCount + introCountDown;
                int newExtroCount = extroCount + extroCountDown;
                if (newIntroCount > introvertsCountLimit) continue;
                if (newExtroCount > extrovertsCountLimit) continue;
                int &target = dp[y+1][newIntroCount][newExtroCount][rowStateDown];
                int candidate = dp[y][introCount][extroCount][rowStateUp]+horizontalHappiness[rowStateDown]+verticalHappiness[rowStateUp][rowStateDown];
                if (candidate > target) {
                  target = candidate;
                  // printf("When dp[y:%d][introCount:%d][extroCount:%d][rowStateUp:%s] == %d,\n\tdeduced dp[y+1:%d][newIntroCount:%d][newExtroCount:%d][rowStateDown:%s] == %d, by horizontalHappiness[rowStateDown:%s] = %d, verticalHappiness[rowStateUp:%s][rowStateDown:%s] == %d\n", y, introCount, extroCount, printRowState(rowStateUp, n).c_str(), dp[y][introCount][extroCount][rowStateUp], y+1, newIntroCount, newExtroCount, printRowState(rowStateDown, n).c_str(), target, printRowState(rowStateDown, n).c_str(), horizontalHappiness[rowStateDown], printRowState(rowStateUp, n).c_str(), printRowState(rowStateDown, n).c_str(), verticalHappiness[rowStateUp][rowStateDown]); 
                  if (target > ans) {
                    ans = target;
                  } 
                }
              }
            }
          }
        }
      }
      
      return ans;
    }
};
