class Solution {
public:
    int hIndex(vector<int>& citations) {
      int n = citations.size();
      vector<int> dp(n+2, 0);
      /*
      It's easy to see that the resultant "h" is less than or equal to "citations.size()".

      "dp[count] == x" means that there're x papers whose citation s are (each) at least "count"
      */
        
      // init 
      for (auto &count : citations) {
        int targetCount = min(count, n);
        ++dp[targetCount];
      }
 
      // loop 
      for (int count = n; count >= 0; --count) {
        dp[count] += dp[count+1];
        if (dp[count] >= count) {
            return count;
        }
      }
      return 0;
    }
};
