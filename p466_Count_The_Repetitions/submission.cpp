/*
test cases
"abaacdbac"
100
"adcbd"
4
"aaa"
20
"aaaaa"
1
*/
class Solution {
public:
    int getMaxRepetitions(string s1, int n1, string s2, int n2) {
        /*
        - Count the occurrence of "s2" in "S1 = [s1, n1]", denoted "baseRep".
        - Answer should be just "floor(baseRep/n2)".
        
        There're several typical relationships between "s1" and "s2" to consider. 
        ``` 
        case#1
        s1 = xxcxxdxxaxxbxx
        s2 = cdab
        ``` 

        ``` 
        case#2
        s1 = xxaxxbxxcxxdxx
        s2 = cdab
        Shall repeat "s1" twice to contain "s2" once
        ``` 

        ``` 
        case#3
        s1 = xxaxxbxxcxxdxx
        s2 = cd(abcd)[n]ab
        Shall repeat "s1" many times to contain "s2" once, but in this case, "s2" MUST CONTAIN a "pattern" which is a subsequence of "s1"
        ``` 
        */
        if (0 == n1 || 0 == n2) return 0;
        int s1Length = s1.length(), s2Length = s2.length();
        vector<int> baseRepr(n1, 0); // by the end of "S1[j]" there're "baseRepr[j]" occurrences of "s2"
        vector<int> yr(n1, 0); // by the end of "S1[j]", the last character of "s2" matched is "S1[j][t] == s2[yr[j]]", where 0 <= t < s1Length 

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
