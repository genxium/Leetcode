class Solution {
public:
    void printCounter(unordered_map<char, int> &counter) {
        for (auto it = counter.begin(); it != counter.end(); ++it) {
            printf("%c: %d\n", it->first, it->second);
        }
        printf("\n");
    }
    
    bool contains(unordered_map<char, int> &sCounter, unordered_map<char, int> &tCounter) {
        // Time-complexity of this method is the size of charset.
        // Intentionally using "unordered_map(hashmap)" instead of "map(treemap)" here to avoid "tree balancing cost", because the "charset" is small and thus unlikely to cause re-hashing.
        if (sCounter.size() < tCounter.size()) {
            return false;
        }
        for (auto it = tCounter.begin(); it != tCounter.end(); ++it) {
            if (sCounter.find(it->first) == sCounter.end()) {
                return false;    
            }
            if (it->second > sCounter[it->first]) {
                return false;
            }
        }
        return true;
    }
    
    string minWindow(string s, string t) {
        if (0 == t.length()) return "";
        unordered_map<char, int> sCounter;
        unordered_map<char, int> tCounter;
        for (int i = 0; i < t.length(); ++i) {
            if (tCounter.find(t[i]) == tCounter.end()) {
                tCounter[t[i]] = 1;
            } else {
                tCounter[t[i]] += 1;
            }
        }
        //printf("The tCounter is\n");
        //printCounter(tCounter);
        int l = 0, r = 0, lAtMinWindow = 0, rAtMinWindow = INT_MAX;
        for (r = 0; r < s.length(); ++r) {
            if (sCounter.find(s[r]) == sCounter.end()) {
                sCounter[s[r]] = 1;
            } else {
                sCounter[s[r]] += 1;
            } 
            //printf("At r == %d, the sCounter is\n", r);
            //printCounter(sCounter);
            
            while (contains(sCounter, tCounter)) {
                //printf("\tsCounter contains tCounter at l == %d\n", l);
                if ((r - l) < (rAtMinWindow - lAtMinWindow)) {
                    lAtMinWindow = l;
                    rAtMinWindow = r;
                }
                --sCounter[s[l]];
                if (sCounter[s[l]] == 0) {
                    sCounter.erase(s[l]);
                }
                ++l;
            }
            // By now, s[l-1, ..., r] contains the characters in t[0, ...], yet s[l, ..., r] doesn't. It's possible that at next step, i.e. "r+1", s[l-1, ..., r+1] contains t[0, ...], however the length "(r+1)-(l-1)" > "r-(l-1)" is NOT a meaningful candidate. Therefore at & after "r+1", the reference "l-1" is NO LONGER USEFUL, making the "window slide" O(n) in time-complexity.
        }
        if (rAtMinWindow == INT_MAX) {
            return "";
        } else {
            return s.substr(lAtMinWindow, rAtMinWindow-lAtMinWindow+1);            
        }
    }
};
