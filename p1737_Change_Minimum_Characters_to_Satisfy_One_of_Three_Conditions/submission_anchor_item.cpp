bool const debug = false;

/*
test cases
"aba"
"caa"
"dabadd"
"cda"
"dee"
"a"
"a"
"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
*/
class Solution {
public:
    int minCharacters(string a, string b) {
        int ans = INT_MAX;
        
        for (char ch = 'a'; ch < 'z'; ++ch) {
            int cand1 = 0;
            // Make all in "string a" not larger than ch.
            for (auto ach : a) {
                if (ach > ch) ++cand1;   
            }
            
            // Make all in "string b" strictly larger than ch.
            for (auto bch : b) {
                if (bch <= ch) ++cand1;
            }
            
            if (cand1 < ans) {
                ans = cand1;
                if (debug) {
                    printf("cond#1, updated ans = %d by ch:%c\n", ans, ch);
                }
            }
        }
        
        for (char ch = 'a'; ch < 'z'; ++ch) {
            int cand2 = 0;
            // Make all in "string b" not larger than ch.
            for (auto bch : b) {
                if (bch > ch) ++cand2;   
            }
            
            // Make all in "string a" strictly larger than ch.
            for (auto ach : a) {
                if (ach <= ch) ++cand2;
            }
            if (cand2 < ans) {
                ans = cand2;
                if (debug) {
                    printf("cond#2, updated ans = %d by ch:%c\n", ans, ch);
                }
            }
        }
        
        for (char ch1 = 'a'; ch1 <= 'z'; ++ch1) {
            for (char ch2 = 'a'; ch2 <= 'z'; ++ch2) {
                // Replace all in "string a" into ch1, all in "string b" into ch2.
                int cand3 = 0;
                for (auto ach : a) {
                    if (ach != ch1) ++cand3;
                }
                for (auto bch : b) {
                    if (bch != ch2) ++cand3;
                }
                if (cand3 < ans) {
                    ans = cand3;
                    if (debug) {
                        printf("updated ans = %d by ch1:%c, ch2:%c\n", ans, ch1, ch2);
                    }
                }
            }
        }
        
        return ans;
    }
};
