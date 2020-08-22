class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        /*
        Denote that a "cycle" is "(n+1) slots". It's obvious that within a "cycle" all tasks must be distinct.
        
        If "(n+1)" is very big, e.g. "(n+1) >= COUNT(DISTINCT(appeared_characters))", it's easy to see that we need "max{count(character) | character in tasks}" cycles.
        --------------------------
        [A: 88, B: 87, C: 24, D: 21, ...], n = 100
        
        (A, B, C, D, ..., %, %, %) - cycle#1
        (A, B, C, D, ..., %, %, %) - cycle#2
        (A, B, C, D, ..., %, %, %) - cycle#3
        ...
        (A, B, %, %, ..., %, %, %) - cycle#87, fullCycleBottom
        (A)                        - cycle#88
        --------------------------
        
        Otherwise, assume that there're "k" distinct characters having equally maximum counts, e.g. "k == 3" and let's prefill the cycles as follows
        
        --------------------------
        [A: 88, B: 88, C: 88, D: 86, E: 85, F: 85, G: 85, H: 84, ...], n = 4
        
        (A, B, C, D)   - cycle#1
        (A, B, C, D)   - cycle#2
        (A, B, C, D)   - cycle#3
        ...
        (A, B, C, D)   - cycle#83
        (A, B, C, D)   - cycle#84
        (A, B, C, D)   - cycle#85
        (A, B, C, D)   - cycle#86
        (A, B, C)      - cycle#87, fullCycleBottom
        (A, B, C)      - cycle#88
        --------------------------
        
        , the trick is to recognize that we should put "E" as follows and there WON'T EVER be a conflict simply because "COUNT("E") < 88".
        
        --------------------------
        [A: 88, B: 88, C: 88, D: 86, E: 85, F: 85, G: 85, H: 84, ...], n = 4
        
        (A, B, C, D, E) - cycle#1
        (A, B, C, D, E) - cycle#2
        (A, B, C, D, E) - cycle#3
        ...
        (A, B, C, D, E) - cycle#83
        (A, B, C, D)    - cycle#84
        (A, B, C, D)    - cycle#85
        (A, B, C, D)    - cycle#86
        (A, B, C, E)    - cycle#87, fullCycleBottom
        (A, B, C, E)    - cycle#88
        --------------------------
        
        This later case actually shows a way to build the most compact solution in detail.
        */
        int maxCount = 0;
        unordered_map<char, int> counter;
        for (auto &ch : tasks) {
            ++counter[ch];
            if (counter[ch] > maxCount) maxCount = counter[ch];
        }
        
        int numFullCycles = (maxCount-1);
        int inevitibleSlotCount = numFullCycles*(n+1);
        int fullCycleBottomExtrudingCount = 0;
        for (auto &it : counter) {
            if (it.second == maxCount) {
                ++fullCycleBottomExtrudingCount;
            }
        }
         
        int tasksCount = tasks.size();
        int ans = max(tasksCount, fullCycleBottomExtrudingCount + inevitibleSlotCount);
        return ans;
    }
};
