class Solution {
public:
  bool isValid(string &s) {
    int leftCnt = 0, rightCnt = 0;
    for (auto &c : s) {
      if (c == '(') ++leftCnt;
      if (c == ')') ++rightCnt;
      if (leftCnt < rightCnt) return false;
    }
    return (leftCnt == rightCnt);
  }  
  
  vector<string> removeInvalidParentheses(string s) {
    vector<string> ans;
    
    unordered_map<string, bool> visited;
    queue<string> q;
    q.push(s);
    
    int longestLength = 0;
    while (!q.empty()) {
      string toEval = q.front(); q.pop();
      if (true == visited[toEval]) continue;
      visited[toEval] = true;
      if (toEval.length() < longestLength) {
        // it's meaningless to traverse below
        continue;
      }
      if (isValid(toEval)) {
        ans.push_back(toEval);
        if (toEval.length() > longestLength) {
          longestLength = toEval.length();
        }
        continue;
      }
      if (toEval.length()-1 < longestLength) {
        // it's meaningless to traverse below
        continue;
      }
      for (int i = 0; i < toEval.length(); ++i) {
        if (toEval[i] == '(' || toEval[i] == ')') {
          string nextToEval = toEval.substr(0, i) + toEval.substr(i+1);          
          q.push(nextToEval);
        }
      }
    }
    
    if (0 == ans.size()) ans.push_back("");
    return ans;
  }
};
