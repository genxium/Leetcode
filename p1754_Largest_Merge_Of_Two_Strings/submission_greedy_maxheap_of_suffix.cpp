class Solution {
public:
    string largestMerge(string word1, string word2) {
        priority_queue<string> q; // the one with larger suffix comes first
        q.push(word1); q.push({word2});
        string ans = "";
        
        while (!q.empty()) {
            string s = q.top(); q.pop();
            ans.push_back(s[0]);
            if (s.length() > 1) {
                q.push(s.substr(1));
            }
        }
        
        return ans;
    }
};
