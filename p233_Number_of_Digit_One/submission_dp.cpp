#define MAXLOGN 11
#define ULL unsigned long long
ULL dp[MAXLOGN];
/*
i   refVal  dp[i]
0   1       0
1   10      1 
2   100     20
3   1000    300
4   10000   4000
*/

void initTotal1sStrictlyUnderPow10() {
  dp[0] = 0u; // strictly under "pow(10, 0) == 1" has no 1 count
  ULL pow10ToI = 1u;
  // pow(10, i) has i trailing 0s
  for (int i = 1; i < MAXLOGN; ++i) {
    dp[i] = 9*dp[i-1]; // count "?xx...x" where "?" represents "[0, 2, 3, ..., 9]"
    dp[i] += dp[i-1] + pow10ToI; // count "1xx...x"
    pow10ToI *= 10u;
  }
}

ULL solve(ULL x) {
  ULL ans = 0;
  while (x > 0) {
    int i = 1;
    ULL pow10ToI = 10u;
    while ( pow10ToI <= x ) {
      ++i; 
      pow10ToI *= 10u;
    }
    // Now that pow(10, i) > x
    --i; pow10ToI /= 10u;
    //printf("pow(10, %d):%u <= x:%u < pow(10, %d):%u\n", i, pow10ToI, x, i+1, 10*pow10ToI);
    // Now that pow(10, i) <= x
    
    ULL j = 1;
    while ( j*pow10ToI <= x ) {
      ++j;
    }
    // Now that j*pow(10, i) > x
    --j;
    // Now that j*pow(10, i) <= x
    //printf("%u*pow(10, %d):%u <= x:%u < %u*pow(10, %d):%u\n\n", j, i, pow10ToI, x, j+1, i, pow10ToI);
    
    if (1 == j) {
      ans += j*dp[i];
      x -= j*pow10ToI;
      if (0 <= x) {
        // count "excessive leading '1's" 
        ans += (1+x); // for example when "x[prev] == 1329", we should have counted "1+329 == 330" here
      }
    } else {
      // 2 <= j
      if (1 <= i) {
        ans += j*dp[i] + pow10ToI;
      } else {
        ans += 1;
      }
      x -= j*pow10ToI;
      // there's no "excessive leading '1's" in this case, e.g. 2329  
    }
  }
  return ans;
}

class Solution {
public:
    Solution() {
      initTotal1sStrictlyUnderPow10();
    }
  
    int countDigitOne(int n) {
      if (n <= 0) return 0;
      /*
      test case #0
      0
      
      test case #1
      1101
      
      test case #2
      1111
      
      test case #3
      1112

      test case #4
      19
      
      test case #5
      20
      
      test case #6
      1120
      
      test case #7
      29

      test case #8
      300

      test case #9
      329

      test case #10
      1329

      test case #11
      2329

      test case #12
      1410065408
      */
      int ans = solve(n);
      return ans;
    }
};
