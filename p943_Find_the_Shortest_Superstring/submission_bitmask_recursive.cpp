bool debug = false;
int const INVALID = -1;
int const MAXN = 12, MAXL = 20;
int const MAXBM = (1 << MAXN);

int const OFFSET = 1;
int cache[MAXN+OFFSET][MAXBM]; // cache[u+OFFSET][remainingBm] is the most reduction when leading by "strs[u]" and the remaining strings indicated by "remainingBm"
int bestLeading[MAXN+OFFSET][MAXBM];

/*
test cases
["alex","loves","leetcode"]
["catg","ctaagt","gcta","ttca","atgcatc"]
["ab","ba"]
["qkawwbuy","hectxljtrm","bcexb","rmnysxnqe","wbuyhectx","jtrmnysx","wwbuyhect"]
*/
class Solution {
public:
    int dfs(int u, int remainingBm, unordered_map<int, unordered_map<int, int>>& adj, int lv) {
        if (0 == remainingBm) return cache[u][remainingBm] = 0;
        int indent = lv*4;
        if (debug) printf("%*sTrying u:%d, remainingBm:%d\n", indent, "", u, remainingBm);
        if (INVALID != cache[u+OFFSET][remainingBm]) return cache[u+OFFSET][remainingBm]; 
        int candReduction = INVALID, candLeading = INVALID; 

        int v = 0;
        int bm = remainingBm;
        while (bm > 0) {
            if ((bm & 1) > 0) {
                int tmp = (INVALID == u ? 0 : adj[u][v]) + dfs(v, remainingBm^(1 << v), adj, lv+1); 
                if (tmp > candReduction) {
                    candReduction = tmp;
                    candLeading = v;    
                }
            }
            bm >>= 1; 
            ++v;
        }

        if (debug) printf("%*sfound for u:%d, remainingBm:%d, candLeading is %d, candReduction is %d\n", indent, "", u, remainingBm, candLeading, candReduction);
        bestLeading[u+OFFSET][remainingBm] = candLeading; 
        return cache[u+OFFSET][remainingBm] = candReduction;
    }

    int overlap(int u, int v, vector<string> &strs) {
        int uLen = strs[u].length();
        char const * vstr = strs[v].c_str();
        for (int i = 0; i < uLen; ++i) {
            char const * occ = strstr(vstr, strs[u].substr(i).c_str());
            if (occ == vstr) {
                return uLen - i;
            }
        }

        return 0;
    }

    string shortestSuperstring(vector<string> &strs) {
        memset(cache, INVALID, sizeof cache);
        memset(bestLeading, INVALID, sizeof bestLeading);
        unordered_map<int, unordered_map<int, int>> adj;
        if (debug) {
            int lengthSum = 0;
            for (int u = 0; u < strs.size(); ++u) {
                lengthSum += strs[u].length();
            }
            printf("lengthSum is %d\n", lengthSum);
        }

        for (int u = 0; u+1 < strs.size(); ++u) {
            for (int v = u+1; v < strs.size(); ++v) {
                adj[u][v] = overlap(u, v, strs);        
                adj[v][u] = overlap(v, u, strs);      
                // if (debug) printf("adj[u:%d][v:%d] == %d, adj[v:%d][u:%d] == %d\n", u, v, adj[u][v], v, u, adj[v][u]);
            } 
        }

        int remainingBm = (1 << strs.size())-1;
        int mostReduction = dfs(INVALID, remainingBm, adj, 0);
        string ans = "";
        
        int u = INVALID;
        int v = bestLeading[u+OFFSET][remainingBm];
        while (INVALID != v) {
            ans += (INVALID == u ? strs[v] : strs[v].substr(adj[u][v]));  
            remainingBm ^= (1 << v);
            u = v;
            v = bestLeading[u+OFFSET][remainingBm];
        }

        return ans;
    } 

};
