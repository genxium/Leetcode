#define LL int64_t

/*
F(0) = 0*a[0]   + 1*a[1] + 2*a[2] + ... + (n-2)*a[n-2] + (n-1)*a[n-1]
F(1) = 0*a[n-1] + 1*a[0] + 2*a[1] + ... + (n-2)*a[n-3] + (n-1)*a[n-2]

F(1) - F(0) = 1*(a[0]-a[1]) + 2*(a[1]-a[2]) + ... + (n-2)*(a[n-3]-a[n-2]) + (n-1)*(a[n-2]-a[n-1])
            = (a[0] + a[1] + a[2] + ... + a[n-2]) - (n-1)*a[n-1]
            = SUM - a[n-1] - (n-1)*a[n-1]
            = SUM - n*a[n-1]
            
Similarly, 

F(2) - F(1) = SUM - n*a[n-2]
...
F(k) - F(k-1) = SUM - n*a[n-k]
*/

/*
test case#1
[-2,0,1]

test case#2
[2147483647,2147483647]

test case#3
[-2147483648,-2147483648]
*/
class Solution {
public:
    int maxRotateFunction(vector<int>& a) {
      int n = a.size();
      if (0 == n) {
        return 0;
      }
      LL sum = 0, ans = INT64_MIN;
      vector<LL> dp(n, 0);
      
      // init
      for (int i = 0; i < n; ++i) {
        sum += a[i];
        dp[0] += (n-1-i)*a[n-1-i];
      }
          
      ans = max(ans, dp[0]);
      
      // loop
      for (int k = 1; k < n; ++k) {
        dp[k] = dp[k-1] + sum - n*(LL)a[n-k];
        ans = max(ans, dp[k]);
      }
    
      return ans;
    }
};
