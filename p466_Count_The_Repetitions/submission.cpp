/*
test case#1
"abaacdbac"
100
"adcbd"
4

test case#2
"aaa"
20
"aaaaa"
1
*/
class Solution {
public:
    int getMaxRepetitions(string s1, int n1, string s2, int n2) {
        /*
        1. Count the occurrence of "s2" in "S1 = [s1, n1]", denoted "baseRep".
        2. Answer should be just "floor(baseRep/n2)".
        
        It's important to note that the counting of "baseRep" is "patternized", e.g. if 
        - "S1[i]" is the "i-th repetition of S1 = [s1, n1] where 1 <= i <= n1", and 
        - "S1[i][u] == s2[s2.length()-1]" is the first "s2 match end" in the whole "S1"
        , then
        - (case#1) it's certain that the next match starts no later than "S1[i+1][t < u] == s2[0]" or otherwise it'd have never started since "S1[0]", MOREOVER if the next match starts with "S1[i+1][t] == s2[0]", then the very first match must also start with "S1[0][t] == s2[0]", HOWEVER
        - (case#2) it's also possible that "S1[i]" contains "S1[i][u' > u] == s2[0]" which starts the next match, thus a difficult case to handle.
        
        For (case#2), the "repetition pattern" DOESN'T just begin with "S1[0][t] == s2[0]", but instead somewhere "S1[i][x] == s2[y]" and ends a single pattern cycle by somewhere "S1[j][x] == s2[y]". Here the difference "j-i" is AT MOST "s2.length()", because whenever "s2" can be fully matched within "S1 = [s1, n1]" there's at least 1 character match per "s1".
        */
        if (0 == n1 || 0 == n2) return 0;
        int s1Length = s1.length(), s2Length = s2.length();
        vector<int> yr(s2Length + 1, 0); // "yr[i] == foo" means that the very last match in "S1[i]" is "S1[i][?] == s2[foo]"
        vector<int> baseRepr(s2Length + 1, 0); // "baseRepr[i] == bar" means that by the end of "S1[i]" there're "bar" matches
        int y = 0, baseRep = 0;
        for (int j = 0; j < n1; j++) {
            for (int x = 0; x < s1Length; ++x) {
                if (s1[x] == s2[y]) {
                    ++y;
                }
                if (y == s2Length) {
                    y = 0;
                    ++baseRep;
                }
            }
            baseRepr[j] = baseRep;
            yr[j] = y;
            for (int i = 0; i < j; i++) {
                if (yr[i] != y) continue;
                // Now that "yr[i]" represents a "very last match in S1[i] with s2" and that match was with "s2[yr[i] == y]", thus a pattern end is found.
                int s1SegsCountInPattern = (j-i);
                int totalS1SegsCountFromPatternStart = (n1-1-i); // It starts with "S1[i]".
                int totalWholePatternsCount = totalS1SegsCountFromPatternStart/s1SegsCountInPattern;
                int prevBaseRep = baseRepr[i];
                int majorBaseRep = (baseRepr[j]-baseRepr[i])*(totalS1SegsCountFromPatternStart/s1SegsCountInPattern);
                int remainBaseRep = baseRepr[i + totalS1SegsCountFromPatternStart%s1SegsCountInPattern]-baseRepr[i];
                return (prevBaseRep + majorBaseRep + remainBaseRep) / n2;
            }
        }
        return baseRepr[n1 - 1] / n2; 
    }
};
