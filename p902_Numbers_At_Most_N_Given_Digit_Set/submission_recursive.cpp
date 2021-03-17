bool debug = false;
typedef uint64_t ULL;

/*
test cases
["1","3","5","7"]
100
["1","4","9"]
1000000000
["7"]
8
["3","4","8"]
4
*/
class Solution {
public:
    int dfs(int i, int dCount, vector<char>& ds, string& nstr) {
        if (i == dCount) return 1;
        if (nstr[i] == '0') return 0;
        auto it = lower_bound(ds.begin(), ds.end(), nstr[i]);
        int lessCnt = (it-ds.begin()); 
        int ret = lessCnt*(int)pow(ds.size(), dCount-i-1); // there're "dCount-i-1" rest digits
        if (it != ds.end() && *it == nstr[i]) {
            ret += dfs(i+1, dCount, ds, nstr); // if choosing a digit equal to nstr[i] and moving on
        }
        return ret;
    }

    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        int m = digits.size();
        vector<char> ds; 
        for (auto d : digits) {
            ds.push_back(d[0]);
        }
        string nstr = to_string(n);
        int dCount = nstr.length();
        int ans = 0; 
        // m + m^2 + m^3 + ... + m^(dCount-1)
        for (int i = 1; i <= dCount-1; ++i) {
            ans += (int)pow(m, i);
        }
        if (debug) printf("ans init to %d\n", ans);
        int sameLenNotLargerCount = dfs(0, dCount, ds, nstr);
        ans += sameLenNotLargerCount;
        return ans;
    }
};
