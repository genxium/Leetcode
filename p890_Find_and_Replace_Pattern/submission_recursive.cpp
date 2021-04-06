class Solution {
public:
    bool isMatched(string& s, string& pat, int i, unordered_map<char, char> &uv, unordered_map<char, char> &vu /* the existing letter mapping*/) {
        if (s.length() != pat.length()) return false;
        if (i == s.length()) return true;
        
        char u = s[i], v = pat[i];
        if (uv.count(u)) {
            return v == uv[u] && isMatched(s, pat, i+1, uv, vu);
        } else if (vu.count(v)) {
            return u == vu[v] && isMatched(s, pat, i+1, uv, vu);
        } else {
            uv[u] = v; vu[v] = u;
            return isMatched(s, pat, i+1, uv, vu);
        }
    }
    
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string> ans;
        for (auto &word : words) {
            unordered_map<char, char> uv, vu;
            bool res = isMatched(word, pattern, 0, uv, vu);
            if (!res) continue;
            ans.push_back(word);
        }
        return ans;
    }
};
