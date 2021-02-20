bool const debug = true;
typedef vector<int> VI;
typedef pair<int, int> PII;

int const MAXN = 12;
int const MAX_VISITED = (1 << MAXN); // All nodes being visited is represented by "1,1,1,...,1"
int const INVALID = -1;
int dp[MAX_VISITED][MAXN]; // dp[visitedMask][ledByNodeId]  

struct PairHash {
    size_t operator() (PII const& val) const {
        return (val.first^val.second);
    }
};

class Solution {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        int n = graph.size();
        memset(dp, INVALID, sizeof dp);

        unordered_set<PII, PairHash> openSet;

        for (int leadingNodeId = 0; leadingNodeId < n; ++leadingNodeId) {
            dp[(1 << leadingNodeId)][leadingNodeId] = 0;
            PII p = {(1 << leadingNodeId), leadingNodeId};
            openSet.insert(p);
        }

        while (!openSet.empty()) {
            auto t = openSet.begin(); 
            int visitedMask = t->first, leadingNodeId = t->second;
            openSet.erase(t);
            
            for (auto newLeadingNodeId : graph[leadingNodeId]) {
                int newVisitedMask = (visitedMask | (1 << newLeadingNodeId));
                int candidate = dp[visitedMask][leadingNodeId] + 1;
                if (INVALID == dp[newVisitedMask][newLeadingNodeId] || candidate < dp[newVisitedMask][newLeadingNodeId]) {
                    // Relaxed.
                    dp[newVisitedMask][newLeadingNodeId] = candidate;
                    /*
                    [WARNING]
                    
                    A pair {newVisitedMask, newLeadingNodeId} might be "reentrant" here, i.e. it was popped out from "openSet" before (possibly multiple times), but as long as it's relaxed now it should re-enter the "openSet". 
                    */
                    openSet.insert({newVisitedMask, newLeadingNodeId});
                }
            }
        }

        int ans = INT_MAX, allVisitedMask = (1 << n)-1;
        for (int leadingNodeId = 0; leadingNodeId < n; ++leadingNodeId) {
            if (INVALID == dp[allVisitedMask][leadingNodeId]) continue; // In fact there won't be such an invalid value.
            ans = min(ans, dp[allVisitedMask][leadingNodeId]);
        }

        return ans;
    }
};
