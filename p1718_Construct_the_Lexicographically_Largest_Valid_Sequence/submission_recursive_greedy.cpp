bool const debug = false;
int const INVALID = -1;

/*
test cases
1
2
3
4
5
10
20
*/
class Solution {
public:
    void dfs(map<int, int, greater<int>> &remainedNums, vector<int> &ans, set<int> &remainedPositions, bool &found, int len) {
        if (found) return;
        if (remainedNums.empty()) {
            found = true;
            return;
        }
        
        auto it = remainedNums.begin();
        while (it != remainedNums.end() && !found) {
            int val = it->first;
            int firstSlot = *remainedPositions.begin();
            if (debug) printf("Checking to place val:%d at firstSlot:%d\n", val, firstSlot);
            if (1 == val) {
                ans[firstSlot] = 1; 
                remainedNums.erase(1);
                remainedPositions.erase(firstSlot);
                dfs(remainedNums, ans, remainedPositions, found, len);
                if (found) return;
                // backtrace
                if (debug) printf("\tBacktracing for val:%d, firstSlot:%d\n", val, firstSlot);
                ans[firstSlot] = INVALID; 
                remainedNums[1] = 1;
                remainedPositions.insert(firstSlot);
                if (debug) printf("\tBacktraced for val:%d, firstSlot:%d\n", val, firstSlot);
                it = remainedNums.upper_bound(val);
            } else {
                int secondSlot = firstSlot + val;
                if (secondSlot >= len || remainedPositions.find(secondSlot) == remainedPositions.end()) {
                    if (debug) printf("\tCouldn't place val:%d at firstSlot:%d\n", val, firstSlot);
                    // couldn't place this number, try next
                    ++it;
                    continue;
                }
                ans[firstSlot] = val; ans[secondSlot] = val;
                remainedNums.erase(val);
                remainedPositions.erase(firstSlot);
                remainedPositions.erase(secondSlot);
                dfs(remainedNums, ans, remainedPositions, found, len);
                if (found) return;
                // backtrace
                if (debug) printf("\tBacktracing for val:%d, firstSlot:%d\n", val, firstSlot);
                ans[firstSlot] = INVALID; ans[secondSlot] = INVALID;
                remainedNums[val] = 2;
                remainedPositions.insert(firstSlot);
                remainedPositions.insert(secondSlot);
                if (debug) printf("\tBacktraced for val:%d, firstSlot:%d\n", val, firstSlot);
                it = remainedNums.upper_bound(val);
            }
        }
    }
    
    vector<int> constructDistancedSequence(int n) {
        if (1 == n) return {1};
        if (2 == n) return {2, 1, 2}; // the only solution
        
        map<int, int, greater<int>> remainedNums;

        int len = 2*n-1;
        
        for (int i = 1; i <= n; ++i) {
            remainedNums[i] = 1;
            if (i > 1) ++remainedNums[i];
        }
        
        set<int> remainedPositions;        
        for (int i = 0; i < len; ++i) remainedPositions.insert(i);
        
        vector<int> ans(len, INVALID);
        // init 
        ans[0] = n; ans[n] = n;
        remainedNums.erase(n);
        remainedPositions.erase(0);
        remainedPositions.erase(n);
        
        bool found = false;
        dfs(remainedNums, ans, remainedPositions, found, len);
        
        return ans;
    }
};
