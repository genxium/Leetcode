bool const debug = false;
int const INTEGER_BIT_LENGTH = sizeof(int)*CHAR_BIT;

/*
test cases
1
5
6
10
1023
1024
1030
1000000000
*/
class Solution {
public:
    string bitRep(int x) {
        if (0 == x) return "0";

        string toRet = "";
        while (x > 0) {
            toRet.push_back((x & 1) ? '1' : '0');
            x >>= 1;
        }

        reverse(toRet.begin(), toRet.end());
        return toRet;
    }

    /*
    Using this "dfs" approach will result in TLE.
    */
    void dfs(int n, int prefixOfN, int accNum, int singleSetBitmask, int &ans, unordered_set<int> &visited) {
        if (visited.find(accNum) != visited.end()) return;

        if (debug) printf("Checking accNum: %s, prefixOfN: %s, singleSetBitmask: %s\n", bitRep(accNum).c_str(), bitRep(prefixOfN).c_str(), bitRep(singleSetBitmask).c_str());

        if (accNum > n) return;

        if (debug) printf("accNum:%d is counted\n", accNum);
        ++ans;
        visited.insert(accNum);

        if (0 == singleSetBitmask) return;

        int thisBitOfN = (n & singleSetBitmask) > 0 ? 1 : 0;
        if (0 < (1 & accNum)) {
            // The last digit of "accNum" is '1', thus the next appended can only be '0'.
            dfs(n, prefixOfN*2 + thisBitOfN, accNum*2, singleSetBitmask/2, ans, visited);
        } else {
            // The last digit of "accNum" is '0'.
            dfs(n, prefixOfN*2 + thisBitOfN, 1+accNum*2, singleSetBitmask/2, ans, visited);
            dfs(n, prefixOfN*2 + thisBitOfN, accNum*2, singleSetBitmask/2, ans, visited);
        }
    }

    int solve(int n) {
        vector<int> dp(INTEGER_BIT_LENGTH, 0); // "dp[i] == x” means that "count of strings of bit-length EXACTLY `i` and without consecutive `1`s" is "x", note that the leading bit can be '0'.

        dp[0] = 1;
        dp[1] = 2; // "0" and "1"

        for (int i = 2; i < INTEGER_BIT_LENGTH; ++i) {
            dp[i] = 
                    dp[i-1]  // appends "0" 
                  + dp[i-2]; // appends "01"
        }

        int i = (int)(log2(n)), ans = 0, prevBit = 0;
        while (i >= 0) {
            if ((n & (1 << i)) != 0) {
                ans += dp[i];
                if (prevBit == 1) {
                    /*
                    For n == "11xxxxxxxxxxxx"
                                ------------
                                  i bits

                    , we only count 0xxxxxxxxxxxxx and 10xxxxxxxxxxxx, then stop moving on because the later 11xxxxxxxxxxxx     
                                     -------------       ------------                                          ------------
                                       (i+1) bits          i bits                                                i bits
                                       
                    is definitely not a valid candidate (including n itself, thus ans--).
                    */
                    ans--;
                    break;
                }
                prevBit = 1;
            } else {
                prevBit = 0;
            }
            i--;
        }
        return ans + 1;
    }
    
    int findIntegers(int n) {
        return solve(n);
    }
};
