#define ANYPOND 1

class Solution {
public:
    vector<int> avoidFlood(vector<int>& rains) {
        int n = rains.size();
        // test case [0]
        // test case [3,5,4,0,1,0,1,5,2,8,9]
        // test case [1,0,2,0,3,0,3,2,1]
        // test case [1,2,0,0,2,1]
        
        unordered_map<int, int> currentlyFulledPonds;
        set<int> cachedPumpingDaysIndex;
        
        vector<int> ans(n, ANYPOND);
        
        for (int i = 0; i < n ;++i) {
            if (rains[i] == 0) {
                cachedPumpingDaysIndex.insert(i);
            } else {
                ans[i] = -1;
                if (currentlyFulledPonds.count(rains[i]) <= 0) {
                    currentlyFulledPonds[rains[i]] = i; // The last occurence index.
                } else {
                    int lastDayFulled = currentlyFulledPonds[rains[i]];
                    auto pumpingDayItr = cachedPumpingDaysIndex.upper_bound(lastDayFulled); // The day we pump "rains[i]" must be AFTER "lastDayFulled".
                    if (pumpingDayItr == cachedPumpingDaysIndex.end()) {
                        return {};
                    }
                    int pumpingDay = *pumpingDayItr;
                    //printf("for rains[i:%d] == %d, lastDayFulled == %d, the proper pumpingDay was %d\n", i, rains[i], lastDayFulled, pumpingDay);
                    ans[pumpingDay] = rains[i];
                    cachedPumpingDaysIndex.erase(pumpingDayItr);
                    currentlyFulledPonds[rains[i]] = i; // The last occurence index.
                }
            }
        }
        return ans;
    }
};
