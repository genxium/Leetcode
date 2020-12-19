typedef vector<int> VI;
bool const debug = false;

/*
test cases
[[1,1],[2,1],[1,1]]
2
3
3
[[1,2],[3,3],[3,1],[3,1],[2,4]]
3
3
6
[[1,4],[1,2],[2,1],[2,1],[3,2],[3,4]]
3
6
7
[[2,4],[2,5],[3,1],[3,2],[3,7],[3,1],[4,4],[1,3],[5,2]]
5
5
7
*/
class Solution {
public:
    int boxDelivering(vector<vector<int>>& boxes, int portsCount, int maxBoxes, int maxWeight) {
        int n = boxes.size();
        vector<int> nextIsDifferentPort(n, 0);
        for (int i = 0; i < n; ++i) {
            if (i+1 == n) continue;
            nextIsDifferentPort[i] = (boxes[i][0] != boxes[i+1][0] ? 1 : 0);
        }
        
        // init
        int l = 0, r = 1, portChangesOfLastBatch = 0, weightOfLastBatch = 0;
        vector<int> dp(n+1, 0); // Compute each "dp[r] == min # of trips to deliver boxes[0, ..., r-1] and back to storage", where the last batch contains "boxes[l, ..., r-1]", it's easy to see that "dp[]" is non-decreasing. Initially "dp[0] === 0", meaning that delivery of 0 items requires 0 trip.
        
        // loop
        while (r <= n) {
            weightOfLastBatch += boxes[r-1][1];
            
            while (r-l > maxBoxes || weightOfLastBatch > maxWeight) {
                if (debug) printf("Before moving l:%d forward, portChangesOfLastBatch:%d, nextIsDifferentPort[l:%d]: %d\n", l, portChangesOfLastBatch, l, nextIsDifferentPort[l]);
                weightOfLastBatch -= boxes[l][1];
                portChangesOfLastBatch -= nextIsDifferentPort[l];
                ++l;
            }
            
            while (l+1 < r && dp[l] == dp[l+1]) {
                /*
                In this case, it's always better to exclude "boxes[l]" from the last batch. Note that "dp[l+1]" regards "boxes[?, ..., l]" as the last batch.
                */
                weightOfLastBatch -= boxes[l][1];
                portChangesOfLastBatch -= nextIsDifferentPort[l];
                ++l; 
            }
            
            // Now that "if (l+1 < r) then dp[l] < dp[l+1]" and "boxes[l]" is included in the last batch
            dp[r] = dp[l] + portChangesOfLastBatch + 2;
            if (debug) printf("Delivering last batch boxes[l:%d, r-1:%d], portChangesOfLastBatch: %d, dp[r:%d] == %d\n", l, r-1, portChangesOfLastBatch, r, dp[r]);
            
            portChangesOfLastBatch += nextIsDifferentPort[r-1]; // We're taking "boxes[r]" into the last batch 
            ++r;
        }
        
        return dp[n];
    }
};
