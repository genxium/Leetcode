bool const debug = false;
int const INVALID = -1;
int const MAXN = 14;
int cache[(MAXN >> 1)+1][(1 << MAXN)];

/*
test cases
[1,2]
[3,4,6,8]
[1,2,3,4,5,6]
[39759,619273,859218,228161,944571,597983,483239,179849,868130,909935,912143,817908,738222,653224]
[481851,31842,817070,452937,627635,712245]
*/
class Solution {
private:
    int gcd(int big, int small) {
        if (big < small) swap(big, small);
        if (small == 0) return big;
        return gcd(small, big%small);
    }

    int dfs(int fixedU, int step, vector<unordered_map<int, int>>& adj, int remainedBm) {
        int m = adj.size(), ret = 0;
        if (INVALID == fixedU) {
            if (INVALID != cache[step][remainedBm]) return cache[step][remainedBm];
            for (int u = 0; u < m; ++u) {
                int mask = (1 << u);
                if (false == (remainedBm & mask)) continue;
                if (debug) printf("%*sStep:%d, Checking {remainedBm:%d, u:%d}\n", (2*(m/2-step)), "", step, remainedBm, u);
                int cand = dfs(u, step, adj, (remainedBm^mask)); 
                ret = max(ret, cand);
            }
            cache[step][remainedBm] = ret;
        } else {
            for (auto &[v, vWeight] : adj[fixedU]) {
                int mask = (1 << v);
                if (false == (remainedBm & mask)) continue;
                if (debug) printf("%*sStep:%d, Checking {remainedBm:%d, u:%d, v:%d, gcdVal:%d}\n", (2*(m/2-step)), "", step, remainedBm, fixedU, v, vWeight);
                int cand = vWeight*step + dfs(INVALID, step-1, adj, (remainedBm^mask)); 
                if (cand > ret) {
                    ret = cand;
                }
            }
        }
        return ret;
    }

public:
    int maxScore(vector<int>& nums) {
        memset(cache, INVALID, sizeof cache);
        int m = nums.size();
        vector<unordered_map<int, int>> adj(m);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                if (j == i) continue;
                int gcdVal = gcd(nums[j], nums[i]);
                adj[i][j] = gcdVal;
                adj[j][i] = gcdVal;
            }
        }

        int remainedBm = (1 << m) - 1, step = (m >> 1);
        int ans = dfs(INVALID, step, adj, remainedBm);
        return ans;
    }
};
