bool const debug = false;
typedef pair<int, int> PII;
int const INVALID = -1;

class Solution {
public:
    int largestSubmatrix(vector<vector<int>>& matrix) {
        int yUpper = matrix.size(), xUpper = matrix[0].size();
        vector<PII> intervals;
        
        for (int x = 0; x < xUpper; ++x) {
            PII active = {INVALID, INVALID};
            for (int y = 0; y < yUpper; ++y) {
                if (matrix[y][x] == 1) {
                    if (active.first == INVALID) active.first = y;
                } else {
                    if (active.first != INVALID && active.second == INVALID) {
                        active.second = y;
                        intervals.push_back(active);
                        active = {INVALID, INVALID};
                    }
                }
            }
            
            // wrap up
            if (active.first != INVALID && active.second == INVALID) {
                active.second = yUpper;
                intervals.push_back(active);
            }
        }    
        
        sort(intervals.begin(), intervals.end(), less<PII>());
        if (debug) {
            for (auto &interval : intervals) {
                printf("[%d, %d)\n", interval.first, interval.second);
            }
        }
        
        int ans = 0;
        map<int, int> sortedEnds;
        for (auto &interval : intervals) {
            int endLower = interval.first;
            int endUpper = interval.second;
            
            int cand = (endUpper-endLower); // only 1 row
            ans = max(cand, ans);
            
            auto endLowerIt = sortedEnds.upper_bound(endLower);
            auto endUpperIt = sortedEnds.end();
            
            int totalSortedEnds = 0;
            for (auto it = endLowerIt; it != endUpperIt; ++it) {
                totalSortedEnds += it->second;
            }
            
            if (debug && endLowerIt != sortedEnds.end()) {
                printf("for current [%d,%d), endLowerIt is {endValue:%d, count:%d}, totalSortedEnds is %d\n", interval.first, interval.second, endLowerIt->first, endLowerIt->second, totalSortedEnds);
            }
            
            for (auto it = endLowerIt; it != endUpperIt; ++it) {
                cand = (min(it->first, endUpper) - endLower)*(totalSortedEnds+1); // height*width
                if (cand > ans) {
                    ans = cand;
                    if (debug) {
                        printf("for current [%d,%d), it {endValue:%d, count:%d} & totalSortedEnds:%d, update ans to %d\n", interval.first, interval.second, it->first, it->second, totalSortedEnds, ans);
                    }
                }
                if (it->first >= endUpper) break; // no need to move on, this is the maximum
                totalSortedEnds -= it->second;
            }
                
            if (sortedEnds.find(endUpper) == sortedEnds.end()) {
                sortedEnds[endUpper] = 1;
            } else {
                ++sortedEnds[endUpper];               
            }
        }
        
        return ans;
    }
};
