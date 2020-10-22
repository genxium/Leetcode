#define MAXN 8
#define MOD 1337
#define ULL uint64_t
ULL pow10[MAXN+1];

class Solution {
private:
  ULL concatWithMirror(ULL i) {
      ULL toRet = i;
      while (i > 0) {
          toRet *= 10;
          toRet += i % 10;
          i /= 10;
      }
      return toRet;
  }
  
  ULL concatWithPrefixMirror(ULL i) {
      ULL toRet = (i / 10);
      while (i > 0) {
          toRet *= 10;
          toRet += i % 10;
          i /= 10;
      }
      return toRet;
  }
public:
  int largestPalindrome(int n) {
    // The largest n-digit number is "pow10[n]-1", thus the largest product by 2 n-digit numbers is "pow10[2n] - 2*pow10[n] + 1", having at most 2*n-1 digits.
    pow10[0] = 1;
    for (int i = 1; i <= n; ++i) {
      pow10[i] = 10*pow10[i-1];
    }
    
    ULL leftHalfLow = pow10[n-1], leftHalfHigh = pow10[n]-1; // "i" should have at least n digits, therefore "leftHalfLow == pow10[n-1]"
    for (ULL i = leftHalfHigh; i >= leftHalfLow; i--) {
      ULL p = concatWithMirror(i);
      /*
      The requirement "j*j >= p" is merely cutting unnecessary checks.
      */
      for (ULL j = leftHalfHigh; j >= leftHalfLow & j*j >= p; j--) {
        if ((p % j) == 0) {
          return p % 1337;
        }
      }
    }
    
    for (ULL i = leftHalfHigh; i >= leftHalfLow; i--) {
      ULL p = concatWithPrefixMirror(i);
      for (ULL j = leftHalfHigh; j >= leftHalfLow & j*j >= p; j--) {
        if ((p % j) == 0) {
          return p % 1337;
        }
      }
    }
    
    return 0;
  }
};
