bool const debug = false;
typedef uint64_t ULL;
/*
test cases
3
19
5
501
100
100000000
79
155800339
*/
class Solution {
public:
    int dfs(int x, ULL target, unordered_map<ULL, ULL>& memo, int depth) {
        if (memo.count(target)) return memo[target];
        if (debug) printf("%*sdfs(target = %d)\n", (depth << 2), "", target);
        if (x > target) {
            ULL byAdding = (target*2 - 1); // target*(x/x)
            ULL bySubing = ((x-target)*2); // x - (x-target)*(x/x)
            return memo[target] = min(byAdding, bySubing);
        }

        // greedy
        ULL val = x;
        int steps = 0;
        while (val < target) {
            val *= x;
            ++steps;
        }

        if (val == target) return memo[target] = steps;

        // now that "val > target"
        int bigApproacher = val, bigAlreadySteps = steps;
        int smallApproacher = val/x, smallAlreadySteps = steps-1;
        int byAdding = smallAlreadySteps + dfs(x, target-smallApproacher, memo, depth+1) + 1; // the last "+1" means prepending a "+" to the beginning of the second half
        int bySubing = INT_MAX;
        if (bigApproacher-target < target) {
            // If "bigApproacher-target > target", e.g. x = 5, target = 61, bigApproacher = 125, therefore "bigApproacher - target = 64 > target", we shouldn't run dfs in this case because it might recur indefinitely (the next time will have bigApproacher' = 125, target' = 64 then "bigApproacher' - target' = 61", hence recurring back). 
            bySubing = bigAlreadySteps + dfs(x, bigApproacher-target, memo, depth+1) + 1; 
        }
        return memo[target] = min(byAdding, bySubing);
    }

    int leastOpsExpressTarget(int x, int target) {
        /*
        First of all, notice that "division" is ONLY USEFUL to form "x/x == 1".
        */
        unordered_map<ULL, ULL> memo;
        return dfs(x, (ULL)target, memo, 0);
    }
};
