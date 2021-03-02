bool debug = false;

typedef uint64_t ULL;
ULL const MOD = 1000000007;
/*
"dp[x]" is the number of such binary trees rooted by value "x", then 

```
dp[x] = SUM{dp[a]*dp[b]}
```

where "x == a*b" and all originally in "arr". It's guaranteed that "arr[i] > 1" hence no need to consider infinite layering.
*/
unordered_map<int, ULL> dp; 

/*
test cases
[2,4]
[2,4,5,10]
[2,3,4,6,12,24]
[2,3,5,6,7,8,12,13,14,19,21,23,26,27,31,34,36,37,40,41,42,45,49,759]
*/
class Solution {
public:
    int numFactoredBinaryTrees(vector<int>& arr) {
        dp.clear();
        int n = arr.size();
        sort(arr.begin(), arr.end());
        if (debug) {
            for (int i = 0; i < n; ++i) {
                printf("%d", arr[i]);
                if (i < n-1) printf(",");
            } puts("");
        }
        
        for (int i = 0; i < n; ++i) {
            int x = arr[i];
            ++dp[x]; // As leaf
            for (int j = 0; j < i; ++j) {
                int a = arr[j];
                if (x % a != 0) continue;
                int b = x/a;
                // By now both "dp[a]" and "dp[b]" should've been fully calculated.
                ULL temp = (dp[a]*dp[b])%MOD;
                dp[x] += temp;
                dp[x] %= MOD;
            }
            
            if (debug) printf("dp[x:%d] == %lu\n", x, dp[x]);
        }
            
        ULL ans = 0;
        for (int i = 0; i < n; ++i) {
            ans += dp[arr[i]];
            ans %= MOD;
        }
        
        return ans;
        
    }
};
