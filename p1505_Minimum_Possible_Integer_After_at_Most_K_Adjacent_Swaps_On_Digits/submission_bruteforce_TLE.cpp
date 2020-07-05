class Solution {
public:
    void findMin(string &str, int k, int stepsUsed, int offset, string &ans) {
        int n = str.length();
        while (stepsUsed < k && offset < n) {
            int upper = offset + (k-stepsUsed+1);
            if (upper > n) upper = n;
        
            char minm = str[offset];
            int minmIdx = offset;
            if (offset > 0 && str[offset] == str[offset-1]) {
                // In this case it's impossible to find another "minm" in str[offset, upper).
            } else {
                for (int i = offset; i < upper; ++i) {
                    if (str[i] < minm) {
                        minm = str[i];
                        minmIdx = i; // use the earlier index in case of a tie
                    }
                }    
            }
            
            //printf("For str:%s, k:%d, stepsUsed:%d, offset:%d, found minmIdx:%d, minm:%c\n", str.c_str(), k, stepsUsed, offset, minmIdx, minm);
            if (minmIdx == offset) {
                offset += 1;
            } else {
                for (int i = minmIdx; i > offset; i--) {
                    swap(str[i], str[i-1]);
                }
                // no backtracking needed, go greedy
                int additionalUsedSteps = (minmIdx - offset);
                stepsUsed += additionalUsedSteps;
                offset += 1;
            }
        }
        ans = str;
    }
    
    string minInteger(string num, int k) {
        string ans;
        findMin(num, k, 0, 0, ans);
        return ans;
    }
};
