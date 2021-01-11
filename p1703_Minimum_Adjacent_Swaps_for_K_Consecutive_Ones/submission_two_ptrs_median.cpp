bool const debug = false;
typedef uint64_t ULL;
typedef int64_t LL;

class Solution {
private: 
    int stepsToMoveToGravity(int g, vector<int> &pos, vector<int>& ps, int l, int r) {
        LL tmp = 
                (LL)(g-l)*(pos[g]-g)-(LL)(ps[g]-ps[l]) // [l, g-1] 
                + 
                (LL)(ps[r]-ps[g+1])-(LL)(r-g-1)*(pos[g]-g); // [g+1, r-1]
        
        return (int)tmp;
    }

public:
    int minMoves(vector<int>& nums, int k) {
        if (1 == k) return 0;
        
        vector<int> pos; // every "nums[pos[i]] == 1"
        
        for (int j = 0; j < nums.size(); ++j) {
            if (!nums[j]) continue;
            pos.push_back(j);
        }
        int n = pos.size();
        if (debug) printf("n == pos.size(): %d\n", n);
        
        int l = 0, r = l+k;
        /*
        For the "1"s within "pos[l, r)" and a chosen "center of gravity pos[g]", any "pos[i]" should end at "pos[g]+(i-g)", by "number of swaps == |pos[i]-(pos[g]+(i-g))| == |(pos[i]-i)-(pos[g]-g)|". Note that "pos[i] >= i" is always true.
        */
        vector<int> ps = {0}; // prefixSum of "(pos[i]-i)"
        for (int i = 0; i < n; ++i) ps.push_back(ps.back()+(pos[i]-i)); 
        
        int ans = INT_MAX;
        while (r <= pos.size()) {
        if (debug) printf("l:%d, r:%d\n", l, r);
            if (k & 1) {
                int g = l+k/2; 
                int cand = stepsToMoveToGravity(g, pos, ps, l, r);
                if (debug) printf("\tcand[g:%d]\n", g, cand);
                ans = min(ans, cand);
            } else {
                int g1 = l+k/2; 
                int cand1 = stepsToMoveToGravity(g1, pos, ps, l, r);
                if (debug) printf("\tcand[g1:%d] == %d\n", g1, cand1);
                ans = min(ans, cand1);

                int g2 = g1-1; 
                int cand2 = stepsToMoveToGravity(g2, pos, ps, l, r);
                if (debug) printf("\tcand[g2:%d] == %d\n", g2, cand2);
                ans = min(ans, cand2);
            }
            ++r;
            ++l;
        }

        return ans;
    }
};

