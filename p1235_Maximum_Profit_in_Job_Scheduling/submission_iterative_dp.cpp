bool debug = false;
int const MAXN = 50000;
typedef vector<int> VI;
typedef int ULL;

int const NOT_TAKEN = 0, TAKEN = 1;
int dp[MAXN][2]; // dp[i][NOT_TAKEN | TAKEN] = accVal

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
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        vector<VI> events;
        for (int i = 0; i < startTime.size(); ++i) {
            events.push_back({startTime[i], endTime[i], profit[i]});
        }

        int n = events.size();
        sort(events.begin(), events.end(), EventCompare());
        
        // init
        memset(dp, 0, sizeof dp);
        for (int i = 0; i < n; ++i) {
            dp[i][NOT_TAKEN] = 0;
            dp[i][TAKEN] = events[i][2];
        }
        
        // loop
        for (int i = 1; i < n; ++i) {
            VI event = events[i];
            
            // Don't take current event.
            dp[i][NOT_TAKEN] = max(dp[i-1][NOT_TAKEN], dp[i-1][TAKEN]);
            
            // Take the current event.
            int prevShouldFinishOnOrBefore = event[0];
            auto it = upper_bound(events.begin(), events.end(), prevShouldFinishOnOrBefore, EventSearch());
            int lastAvailableEventId = (int)(it-events.begin())-1;
            if (lastAvailableEventId < 0) {
                // no event ends strictly before "event[0]"
                continue;
            }
            dp[i][TAKEN] = max(dp[i][TAKEN], dp[lastAvailableEventId][TAKEN] + event[2]);
            dp[i][TAKEN] = max(dp[i][TAKEN], dp[lastAvailableEventId][NOT_TAKEN] + event[2]);
        }
        
        int ans = max(dp[n-1][TAKEN], dp[n-1][NOT_TAKEN]);
        return ans;
    }
};
