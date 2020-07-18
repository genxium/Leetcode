class Solution {
public:
    void dfs(string& exp, unordered_map<string, list<int>> &cache, int &origLength, vector<int> &ans) {
      // find any operator between [exp.begin(), exp.end())
      bool foundAnyOperator = false;
      for (int i = 0; i < exp.length(); ++i) {
        char c = exp[i];
        if ('+' == c || '-' == c || '*' == c) {
          string lhs = exp.substr(0, i);
          string rhs = exp.substr(i+1, exp.length()-i-1); 

          if (cache.find(lhs) == cache.end()) {
            dfs(lhs, cache, origLength, ans);
          }
          if (cache.find(rhs) == cache.end()) {
            dfs(rhs, cache, origLength, ans);
          }
          auto lhsResults = cache[lhs];
          auto rhsResults = cache[rhs];

          for (auto &lhsResult : lhsResults) {
            for (auto &rhsResult : rhsResults) {
              int candidate = 0;
              if ('+' == c) candidate = (lhsResult + rhsResult);
              if ('-' == c) candidate = (lhsResult - rhsResult);
              if ('*' == c) candidate = (lhsResult * rhsResult);
              if (exp.length() == origLength) {
                ans.push_back(candidate);
              } else {
                cache[exp].push_back(candidate); 
              }
            }
          }
          foundAnyOperator = true;
        }
      }
      
      if (false == foundAnyOperator) {
        if (exp.length() == origLength) {
          ans.push_back(atoi(exp.c_str()));
        } else {
          cache[exp].push_back(atoi(exp.c_str()));
        }
      }
    }
  
    vector<int> diffWaysToCompute(string input) {
      /*
      test case #1
      "15+7-6*24"
      
      */
      // we assume that "input" is always a valid expression
      unordered_map<string, list<int>> cache;
      vector<int> ans; // no need to deduplicate
      int origLength = input.length();
      dfs(input, cache, origLength, ans);
      return ans;
    }
};
