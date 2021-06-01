bool const debug = false;
typedef vector<int> VI;

/*
test cases
5
[[2,1],[4,1]]
6
[]
10
[[5,3],[2,5],[7,4],[10,3]]
10
[[6,0],[5,2],[7,0],[9,1],[2,4],[3,4],[4,0],[8,2],[10,0]]
6
[[5,0],[6,0]]
*/
class Solution {
public:
    int maxBuilding(int n, vector<vector<int>>& r) {        
        int m = r.size();
        if (debug) printf("m is %d\n", m);
        if (0 == m) return n-1; 
            
        sort(r.begin(), r.end(), [](VI const & a, VI const & b) -> bool {
          return a[0] < b[0];
        });
        vector<VI> h(m, VI(2, 0)); // h[i] = {id, maxActualHeight} 
        map<int, int> counter; // counts remaining "r[i][0] + r[i][1]"
        for (int i = 0; i < m; ++i) {
            /*
            The buildings are ASYMMETRIC, i.e. when determining each "h[i]", we look at "h[i-1]" as well as "r[j > i] records", it's implied by problem description that "h[i][1]-r[j][1] <= (r[j][0]-h[i][0])" is required (such that it's possible to decrease to all restricted heights on the right), thus "h[i][1] - h[i][0] <= min(r[j][0]+r[j][1] | j > i)"
            */
            ++counter[r[i][0]+r[i][1]];
        }
        
        int ans = 0;
        for (int i = 0; i < m; ++i) {
            h[i][0] = r[i][0];
            
            --counter[r[i][0]+r[i][1]];
            if (0 == counter[r[i][0]+r[i][1]]) counter.erase(r[i][0]+r[i][1]);
            
            int result = r[i][1];
            if (i > 0) {
                int cand = h[i-1][1]+(h[i][0]-h[i-1][0]);
                result = min(result, cand);
            } else {
                int cand = h[i][0]-1; // because "id == 1" has actual height "0"
                result = min(result, cand);
            }
            
            if (!counter.empty()) {
                int cand = counter.begin()->first - h[i][0];
                result = min(result, cand);
            }
            
            h[i][1] = result;
            
            if (debug) printf("h[i:%d][1] is %d\n", i, h[i][1]);
            ans = max(ans, result);
        }
        
        // Check possible heights between each "h[i], h[i+1]", including "h[i+1] -> last"
        for (int i = 0; i < m; ++i) {
            if (i+1 < m) {
                int dTot = h[i+1][0]-h[i][0];
                /*
                Assume that "(x, y)" is the highest in between (h[i][0], h[i+1][0]), if "y > max(h[i][1], h[i+1][1])" we should have 
                -- (y-h[i][1]) = (x-h[i][0])
                -- (y-h[i+1][1]) = (h[i+1][0]-x)
                */
                int y = (h[i][1]+h[i+1][1]+dTot)/2;
                ans = max(ans, y);
            } else {
                // To the last id, i.e. "n"
                int y = h[i][1] + (n-h[i][0]);
                ans = max(ans, y);
            }
        }
        
        // Check also between "(1, h[i][0])"
        int dTot = (h[0][0]-1);
        int y = (0+h[0][1]+dTot)/2;
        ans = max(ans, y);
        
        return ans;
    }
};
