bool debug = false;
typedef vector<int> VI;

class EventCompare {
public:
    bool operator() (VI const& lhs, VI const& rhs) {
        if (lhs[1] != rhs[1]) {
            return lhs[1] < rhs[1];
        }

        if (lhs[2] != rhs[2]) {
            return lhs[2] > rhs[2];
        }

        // same ending day and same value
        return lhs[0] > rhs[0]; 
    }
};

class EventSearch {
public:
    bool operator() (int const& lhs, VI const& rhs) {
        return lhs < rhs[1];
    }
};

class Solution {
public:
    int maxValue(vector<vector<int>>& events, int k) {
        int n = events.size();
        sort(events.begin(), events.end(), EventCompare());
        
        // init
        unordered_map<int, unordered_map<int, int>> dp; // dp[i][finishedCount] = accVal
        for (int i = 0; i < n; ++i) {
            dp[i][0] = 0;
            dp[i][1] = events[i][2];
        }
        
        // loop
        for (int i = 1; i < n; ++i) {
            VI event = events[i];
            
            // Don't take current event.
            for (auto &[finishedCount, accVal] : dp[i-1]) {
                dp[i][finishedCount] = max(dp[i][finishedCount], accVal);
            }
            
            // Take the current event.
            int prevShouldFinishOnOrBeforeDay = event[0]-1;
            auto it = upper_bound(events.begin(), events.end(), prevShouldFinishOnOrBeforeDay, EventSearch());
            int lastAvailableEventId = (int)(it-events.begin())-1;
            if (lastAvailableEventId < 0) {
                // no event ends strictly before "event[0]"
                continue;
            }
            auto &thatEvent = events[lastAvailableEventId];
            for (auto &[finishedCount, accVal] : dp[lastAvailableEventId]) {
                if (finishedCount == k) continue;
                int newFinishedCount = finishedCount+1;
                dp[i][newFinishedCount] = max(dp[i][newFinishedCount], accVal+event[2]);
            }
        }
        
        int ans = 0;
        for (auto &[finishedCount, accVal] : dp[n-1]) {
            ans = max(ans, accVal);
        }
        
        return ans;
    }
};

