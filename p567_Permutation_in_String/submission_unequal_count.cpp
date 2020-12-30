bool const debug = false;

/*
test cases
"ab"
"eidbaooo"
"ab"
"eidboaoo"
"adc"
"dcda"
*/
class Solution {
public:
    bool checkInclusion(string s1, string s2) {    
        if (s1.length() > s2.length()) return false;
        
        unordered_map<int, int> counter1;
        for (auto &ch : s1) ++counter1[ch];
        
        unordered_map<int, int> counter2;
        int l = 0, r = s1.length();
        for (int i = l; i < r; ++i) {
            ++counter2[s2[i]];
        }
        
        int unequalCount = 0;
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            if (counter1[ch] != counter2[ch]) ++unequalCount;
        }
        
        if (0 == unequalCount) return true;
        
        if (debug) printf("#1 unequalCount:%d\n", unequalCount);
        
        while (r < s2.length()) {
            if (s2[r] != s2[l]) {
                if (counter2[s2[r]] == counter1[s2[r]]-1) --unequalCount;
                else if (counter2[s2[r]] == counter1[s2[r]]) ++unequalCount;
                else; // remains
                
                if (counter2[s2[l]] == counter1[s2[l]]+1) --unequalCount;
                else if (counter2[s2[l]] == counter1[s2[l]]) ++unequalCount;
                else; // remains
                
                if (debug) printf("#2, evicting counter2[s2[l:%d]:%c] == %d, inserting counter2[s2[r:%d]:%c] == %d, unequalCount:%d\n", l, s2[l], counter2[s2[l]], r, s2[r], counter2[s2[r]], unequalCount);
                
                if (0 == unequalCount) return true;
                    
                ++counter2[s2[r]];
                --counter2[s2[l]];
            }
            ++l;
            ++r;
        }
        
        return false;
    }
};
