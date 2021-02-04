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
        /*
         Note that "intervals" are

         - all parallel to y-axis
         - sorted primarily by their "starting-y" and secondly by "ending-y"

         , which means that upon visiting any "intervals[k]" in the loop below, there're at most "k" entries in "sortedEnds"
         */
        map<int, int> sortedEnds;
        for (auto &interval : intervals) {
            int currentStartingY = interval.first;
            int currentEndingY = interval.second;
            
            int cand = (currentEndingY-currentStartingY); // only 1 row
            ans = max(cand, ans);
            
            auto prevEndingLowerIt = sortedEnds.upper_bound(currentStartingY);
            
            int effPrevEndingsCount = 0;
            for (auto it = prevEndingLowerIt; it != sortedEnds.end(); ++it) {
                effPrevEndingsCount += it->second;
            }
            
            if (debug && prevEndingLowerIt != sortedEnds.end()) {
                printf("for current [%d,%d), prevEndingLowerIt is {endValue:%d, count:%d}, effPrevEndingsCount is %d\n", interval.first, interval.second, prevEndingLowerIt->first, prevEndingLowerIt->second, effPrevEndingsCount);
            }
            
            for (auto it = prevEndingLowerIt; it != sortedEnds.end(); ++it) {
                cand = (min(it->first, currentEndingY) - currentStartingY)*(effPrevEndingsCount+1); // height*width
                if (cand > ans) {
                    ans = cand;
                    if (debug) {
                        printf("for current [%d,%d), it {endValue:%d, count:%d} & effPrevEndingsCount:%d, update ans to %d\n", interval.first, interval.second, it->first, it->second, effPrevEndingsCount, ans);
                    }
                }
                if (it->first >= currentEndingY) break; // no need to move on, this is the maximum
                effPrevEndingsCount -= it->second;
            }
                
            ++sortedEnds[currentEndingY];               
        }
        
        return ans;
    }
};
