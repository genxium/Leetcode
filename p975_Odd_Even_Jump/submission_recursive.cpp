bool debug = false;
int const INVALID = -1;

/*
test cases
[10,13,12,14,15]
[2,3,1,1,4]
[5,1,3,4,2]
*/
class Solution {
public:
    int dfs(int parity, int i, vector<vector<int>> &nextStep, vector<vector<int>> &reachable) {
        if (INVALID != reachable[parity][i]) return reachable[parity][i];
        if (i == nextStep[parity][i]) return reachable[parity][i] = i;
        return reachable[parity][i] = dfs(1-parity, nextStep[parity][i], nextStep, reachable);
    }
    
    int oddEvenJumps(vector<int>& arr) {
        int n = arr.size();
        int ODD = 1, EVEN = 0;
        vector<vector<int>> nextStep(2, vector<int>(n, INVALID)); // nextStep[EVEN | ODD][i] == j
        
        map<int, int> suffixValToPos;
        
        for (int i = n-1; i >= 0; --i) {
            // Calculate by definition of this problem.
            int jForEven = i, jForOdd = i;
            auto itLower = suffixValToPos.lower_bound(arr[i]);
            if (itLower != suffixValToPos.end()) {
                jForOdd = itLower->second;
            }
            if (itLower->first == arr[i]) {
                jForEven = itLower->second;
            } else {
                if (itLower != suffixValToPos.begin()) {
                    --itLower;
                    jForEven = itLower->second;
                }
            }
            nextStep[ODD][i] = jForOdd;
            nextStep[EVEN][i] = jForEven;
            
            if (debug) printf("nextStep[ODD][i:%d] = %d, nextStep[EVEN][i:%d] = %d\n", i, nextStep[ODD][i], i, nextStep[EVEN][i]);
            suffixValToPos[arr[i]] = i;
        }
        
        vector<vector<int>> reachable(2, vector<int>(n, INVALID)); // reachable[EVEN | ODD][i] == the furthest index reachable if starting from i as EVEN | ODD step
        
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            dfs(ODD, i, nextStep, reachable);
            if (debug) printf("reachable[ODD][i:%d] = %d, reachable[EVEN][i:%d] = %d\n", i, reachable[ODD][i], i, reachable[EVEN][i]);
            if (reachable[ODD][i] == n-1) ++ans;
        }
        return ans;
    }
};
