/*
test cases
"ab"
"ba"
"abc"
"bca"
"abac"
"baca"
"aabc"
"abca"
"abccaacceecdeea"
"bcaacceeccdeaae"
"cdebcdeadedaaaebfbcf"
"baaddacfedebefdabecc"
*/
class Solution {
public:
    int bfsTLE(string &A, string &B) {
        int n = A.length();
        
        unordered_set<string> openSet, closedSet;
        int lv = 0;
        queue<string> q;
        q.push(A); openSet.insert(A);
        
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                auto expander = q.front(); q.pop();
                if (expander == B) return lv;
                closedSet.insert(expander);
                openSet.erase(expander);
                for (int i = 0; i < n; ++i) {
                    if (expander[i] == B[i]) continue; // Any "swapS" must at least reduce the mismatch count by 1.
                    for (int j = 0; j < n; ++j) {
                        if (j == i) continue;
                        if (expander[j] == B[j]) continue; // Any "swapS" must not increase the mismatch count.
                        if (expander[j] != B[i]) continue; 
                        string cand = swapS(expander, i, j);
                        if (openSet.count(cand) || closedSet.count(cand)) continue;
                        q.push(cand);
                        openSet.insert(cand);
                    }
                }                
            }
            ++lv;
        }
        
        return lv;
    }
    
    int bfs(string &A, string &B) {
        int n = A.length();
        
        unordered_set<string> openSet, closedSet;
        int lv = 0;
        queue<string> q;
        q.push(A); openSet.insert(A);
        
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                auto expander = q.front(); q.pop();
                if (expander == B) return lv;
                closedSet.insert(expander);
                openSet.erase(expander);
                int i = 0;
                /*
                The reason we can avoid "traversing all (i, j) pairs" like in "bfsTLE" is that 
                - once "expander[i] != B[i]", if there're "j1 != i, j2 != i, ..." sufficing "expander[j1] == B[i] && expander[j1] != B[j1], expander[j2] == B[i] && expander[j2] != B[j2], ...", and draw an arrow from each "j1, j2, ..." to "i", and
                - there'll also arrows starting from "i" to "expander[i] == B[k1] && expander[k1] != B[k1], expander[i] == B[k2] && expander[k2] != B[k2], ...", and
                - intuitively for each "i" the in-degree will equal the out-degree, THEN 
                - each "swap(j*, i)" is equivalent to following an arrow which at least reducing the mismatch count by 1, and
                - each "i" is contained in some "cycles", and
                - if "cycle-A" doesn't share any "i" with "cycle-B", it DOESN'T MATTER which cycle do we start with searching, THEREFORE
                - if we make sure that every "cycle" that contains the first "i" where "expander[i] == B[i]" is covered at this level, the optimal solution will automatically be included.
                */
                while (expander[i] == B[i]) ++i;
                for (int j = i+1; j < n; ++j) {
                    if (expander[j] == B[j]) continue; // Any "swapS" must not increase the mismatch count.
                    if (expander[j] != B[i]) continue; 
                    string cand = swapS(expander, i, j);
                    if (openSet.count(cand) || closedSet.count(cand)) continue;
                    q.push(cand);
                    openSet.insert(cand);
                }           
            }
            ++lv;
        }
        
        return lv;
    }

    int kSimilarity(string A, string B) {
        return bfs(A, B);
    }
    
    string swapS(string s, int i, int j) {
        string ret(s);
        swap(ret[i], ret[j]);
        return ret;
    }
};
