class Solution {
public:
    int nthSuperUglyNumber(int n, vector<int>& primes) {
        priority_queue<int, vector<int>, greater<int>> openHeap;
        int cnt = 0;
        
        // init
        openHeap.push(1);
        int ans = 0;
        
        // loop
        while (false == openHeap.empty() && cnt < n) {
            int expander = openHeap.top(); openHeap.pop();
            if (expander == ans) {
                // remove redundant "expander"s
                continue;
            }
            ans = expander;
            ++cnt;
            for (int p : primes) {
                if (INT_MAX/p < expander) {
                    continue;
                }
                openHeap.push(expander*p);
            }
        }
        
        return ans;
    }
};
