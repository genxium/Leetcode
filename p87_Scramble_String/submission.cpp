class Solution {
public:
    void printCount(unordered_map<char, int> &m) {
        for (auto it : m) {
            printf("\t%c:%d\n", it.first, it.second);
        }
        printf("\n");
    }
    
    bool sameCounts(unordered_map<char, int> &m1, unordered_map<char, int> &m2) {
        if (m1.size() != m2.size()) return false;
        for (auto it : m1) {
            if (m2.find(it.first) == m2.end() || m2[it.first] != it.second) return false;
        }
        return true;
    }
    
    bool search(string &s1, int lClosed1, int rOpen1, string &s2, int lClosed2, int rOpen2) {
        /*
        If there exists a way to make "scramble(s1, [nodeIdx1, ..., nodeIdxN]) == s2", then
        we must be able to find k1 in [lClose1, rOpen1) & k2 in [lClosed2, rOpen2) such that 
        ```
        either 
        
        characterCount(s1, [lClosed1, k1)) == characterCount(s2, [lClosed2, k2)) &&
        characterCount(s1, [k1, rOpen1)) == characterCount(s2, [k2, rOpen2))
        
        or 
                
        characterCount(s1, [lClosed1, k1)) == characterCount(s2, [k2, rOpen2)) &&
        characterCount(s1, [k1, rOpen1)) == characterCount(s2, [lClosed2, k2))
        */
        
        if (rOpen1-lClosed1 == 1) {
            return (s1[lClosed1] == s2[lClosed2]);
        }
        
        // covering the special case "k1 == rOpen1"
        bool wholeMatched = true;
        int cur1 = lClosed1, cur2 = lClosed2;
        while (cur1 < rOpen1 && cur2 < rOpen2) {
            if (s1[cur1] != s2[cur2]) {
                wholeMatched = false;
                break;
            }
            ++cur1;
            ++cur2;
        }
        if (wholeMatched) {
            return true;
        }
        
        // dive into normal cases "k1 in [lClose1, rOpen1) & k2 in [lClosed2, rOpen2)"
        unordered_map<char, int> s1LeftCount, s1RightCount;
        unordered_map<char, int> s2LeftCount, s2RightCount, rs2LeftCount, rs2RightCount;
        
        // init
        for (auto val : s1) {
            if (s1RightCount.count(val) > 0) {
                s1RightCount[val]++;
            } else {
                s1RightCount[val] = 1;
            }
        }
        
        for (auto val : s2) {
            if (s2RightCount.count(val) > 0) {
                s2RightCount[val]++;
                rs2LeftCount[val]++;
            } else {
                s2RightCount[val] = 1;
                rs2LeftCount[val] = 1;
            }
        }

        // loop, the counts in "s1RightCount", "s2RightCount", "rs2LeftCount" will decrease as "k1" increases 
        bool existsAnyRecursiveMatch = false;
        
        for (int k1 = lClosed1+1; k1 < rOpen1; ++k1) {
            int countOfLeft = (k1-lClosed1);
            int k2 = lClosed2+countOfLeft;
            int rk2 = rOpen2-countOfLeft;
            
            char s1Ch = s1[k1-1], s2Ch = s2[k2-1], rs2Ch = s2[rk2];
            if (s1LeftCount.count(s1Ch) > 0) {
                s1LeftCount[s1Ch]++;
            } else {
                s1LeftCount[s1Ch] = 1;
            }
            --s1RightCount[s1Ch];
            if (s1RightCount[s1Ch] == 0) s1RightCount.erase(s1Ch);
            
            if (s2LeftCount.count(s2Ch) > 0) {
                s2LeftCount[s2Ch]++;
            } else {
                s2LeftCount[s2Ch] = 1;
            }
            --s2RightCount[s2Ch];
            if (s2RightCount[s2Ch] == 0) s2RightCount.erase(s2Ch);

            if (rs2RightCount.count(rs2Ch) > 0) {
                rs2RightCount[rs2Ch]++;
            } else {
                rs2RightCount[rs2Ch] = 1;
            }
            --rs2LeftCount[rs2Ch];
            if (rs2LeftCount[rs2Ch] == 0) rs2LeftCount.erase(rs2Ch);
            /*
            printf("At k1 == %d, [lClosed1:%d, rOpen1:%d), k2 == %d, [lClosed2:%d, rOpen2:%d)\n", k1, lClosed1, rOpen1, k2, lClosed2, rOpen2);
            printf("s1LeftCount:\n");
            printCount(s1LeftCount);
            printf("s1RightCount:\n");
            printCount(s1RightCount);
            
            printf("s2LeftCount:\n");
            printCount(s2LeftCount);
            printf("s2RightCount:\n");
            printCount(s2RightCount);
            
            printf("rs2LeftCount:\n");
            printCount(rs2LeftCount);
            printf("rs2RightCount:\n");
            printCount(rs2RightCount);
            */
            // match case#1
            if (sameCounts(s1LeftCount, s2LeftCount) && sameCounts(s1RightCount, s2RightCount)) {
                //printf("found k1 == %d in [lClosed1:%d, rOpen1:%d) for match case#1\n", k1, lClosed1, rOpen1);
                bool leftRecursiveMatch = search(s1, lClosed1, k1, s2, lClosed2, k2);
                bool rightRecursiveMatch = search(s1, k1, rOpen1, s2, k2, rOpen2);
                if (leftRecursiveMatch && rightRecursiveMatch) {
                    existsAnyRecursiveMatch = true;
                    break;
                }
            }
            
            // match case#2
            if (sameCounts(s1LeftCount, rs2RightCount) && sameCounts(s1RightCount, rs2LeftCount)) {
                //printf("found k1 == %d in [lClosed1:%d, rOpen1:%d) for match case#2\n", k1, lClosed1, rOpen1);
                bool leftRecursiveMatch = search(s1, lClosed1, k1, s2, rk2, rOpen2);
                bool rightRecursiveMatch = search(s1, k1, rOpen1, s2, lClosed2, rk2);
                if (leftRecursiveMatch && rightRecursiveMatch) {
                    existsAnyRecursiveMatch = true;
                    break;
                }
            }
        }
        
        return existsAnyRecursiveMatch;
    }
    
    bool isScramble(string s1, string s2) {
        /*
        test case #1
        "abb"
        "bba"
        
        */
        if (s1.length() != s2.length()) {
            return false;
        }
        if (0 == s1.length()) {
            return true;
        }
        
        bool ans = search(s1, 0, s1.length(), s2, 0, s2.length());
        return ans;
    }
};
