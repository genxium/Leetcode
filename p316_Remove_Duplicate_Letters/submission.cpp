#define MAXC 27

class Solution {
public:
    string removeDuplicateLetters(string s) {
        unordered_map<int, int> countsInFuture;
        bool visited[MAXC];
        memset(visited, false, sizeof(visited));
        
        // init
        for (int i = 0; i < s.length(); ++i) {
            int cIdx = s[i]-'a';
            ++countsInFuture[cIdx];
        }
        string ans = "";
        
        // loop
        for (int i = 0; i < s.length(); ++i) {
            int cIdx = s[i]-'a';
            --countsInFuture[cIdx];
            
            if (visited[cIdx]) {
                continue;
            }
            
            while (!ans.empty()) {
                int cIdxOfCurrentBack = (ans.back() - 'a');
                if (cIdxOfCurrentBack > cIdx && countsInFuture[cIdxOfCurrentBack] > 0) {
                    /*
                    In this case, "ans.back() > s[i]" and 'ans.back()' will re-appear in the future. 
                    */
                    ans.pop_back();
                    visited[cIdxOfCurrentBack] = false;
                } else {
                    /*
                    In this case, EVERY 'ans[k]' in "ans" suffices either
                    - "ans[k] < s[i]", or
                    - "ans[k] > s[i]" but "ans[k]" just won't re-appear in the future, i.e. not only 'ans.back()'. The proof can be done by math induction.
                    */
                    break;
                }
            }

            ans.push_back(s[i]);
            visited[cIdx] = true;
        }
        
        return ans;
    }
};
