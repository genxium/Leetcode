class Solution {
public:
    string rotate(string s, int x) {
        reverse(s.begin(), s.end());
        reverse(s.begin(), s.begin()+x);
        reverse(s.begin()+x, s.end());
        return s;
    }
    
    string add(string s, int a) {
        for (int i = 1; i < s.length(); i += 2) {
            s[i] = '0' + (s[i]-'0'+a)%10;
        }
        return s;
    }
    
    void dfs(string s, unordered_set<string> &visited, int a, int b, string &ans) {
        if (visited.find(s) != visited.end()) {
            return;
        }
        ans = min(ans, s);
        visited.insert(s);
        dfs(rotate(s, b), visited, a, b, ans);
        dfs(add(s, a), visited, a, b, ans);
    }
    
    string findLexSmallestString(string s, int a, int b) {
        string ans = s;
        unordered_set<string> visited;
        dfs(s, visited, a, b, ans);
        return ans;
    }
};
