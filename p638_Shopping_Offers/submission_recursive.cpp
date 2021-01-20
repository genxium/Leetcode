int const debug = false;
int const BASE = 7, INVALID = -1;
int const MAXENCODEDNEEDS = 823542; // 6*7^6+6*7^5+6*7^4+6*7^3+6*7^2+6*7^1+6
int cache[MAXENCODEDNEEDS];

/*
test cases
[2,5]
[[3,0,5],[1,2,10]]
[3,2]
[2,3,4]
[[1,1,0,4],[2,2,1,9]]
[1,2,1]
[3,10,5,7,7,3]
[[3,2,2,6,2,5,30],[4,6,5,2,3,2,12],[1,0,4,6,3,3,23],[0,5,3,2,4,2,8],[4,1,2,6,3,2,33],[3,0,6,5,5,2,33],[1,4,4,0,2,6,10],[5,1,0,5,6,3,16],[3,4,5,6,3,5,5],[6,0,6,0,5,5,22],[3,0,4,4,2,6,15],[5,4,2,3,5,0,9],[4,4,5,5,6,1,23],[2,6,5,6,6,4,4],[5,0,2,1,0,4,21],[1,4,6,6,1,6,4],[4,0,5,6,2,2,17],[1,3,5,1,3,3,25],[0,1,4,5,6,4,7],[0,2,0,6,4,4,17],[3,2,3,5,2,2,27],[2,2,6,1,5,4,31],[6,6,0,0,5,2,6],[4,6,6,2,6,6,2],[1,2,4,4,4,6,22],[4,6,4,4,1,4,2],[6,6,5,6,3,5,31],[6,5,0,4,2,5,8],[2,2,4,3,4,6,24],[6,5,6,1,4,4,25],[5,6,1,3,3,6,2],[5,2,4,4,0,2,6],[3,0,4,3,3,6,27],[5,5,3,3,4,3,10],[1,5,1,5,2,2,9],[3,2,1,1,4,5,8],[3,5,3,6,6,5,13],[0,4,6,5,3,1,17],[5,2,4,6,5,5,23],[0,5,3,1,0,4,20],[3,0,0,4,6,1,28],[3,0,1,3,4,0,3],[5,6,6,3,1,1,22],[6,4,3,4,2,0,10],[1,2,0,2,1,5,28],[5,6,5,6,6,6,8],[2,4,6,0,4,3,4],[3,2,5,1,6,1,15],[0,6,2,1,2,2,7],[1,0,5,4,5,3,21],[4,2,3,6,4,2,29],[1,4,2,3,6,0,28],[4,1,0,4,1,3,22],[1,3,4,0,2,2,27],[0,3,0,2,3,3,18],[0,3,5,0,5,0,31],[5,0,0,0,6,5,33],[5,1,0,6,3,2,32],[3,4,1,1,5,6,29],[1,2,1,1,4,6,34],[2,6,4,5,4,2,24],[4,1,2,2,0,0,22],[3,2,4,4,4,4,7],[2,3,2,3,4,1,8],[1,1,2,3,5,5,25],[3,0,5,0,6,1,17],[0,1,5,1,2,3,16],[6,5,2,0,6,2,32],[3,4,4,2,5,0,23],[3,3,4,6,3,2,17],[1,3,3,1,3,1,14],[5,2,4,2,0,0,11],[0,2,4,1,6,1,16],[2,6,1,4,2,5,20],[4,0,6,5,2,0,19],[3,1,2,1,4,0,8],[2,5,4,0,0,1,19],[5,0,4,5,5,5,23],[5,1,4,2,1,1,34],[1,1,2,4,0,3,3],[2,3,5,5,1,2,24],[6,1,5,2,1,2,18],[4,5,3,3,6,5,26],[6,6,2,0,0,2,32],[6,5,6,1,6,4,15],[0,6,3,5,5,6,5],[5,2,6,2,5,3,24],[5,6,0,4,6,2,7],[4,2,5,3,1,5,8],[5,2,2,6,0,3,34],[5,3,0,3,3,0,5],[1,1,0,6,0,0,21],[6,0,4,2,3,2,13],[2,0,1,0,1,4,32],[1,0,4,4,1,5,2],[4,5,3,6,6,1,34],[6,2,5,4,2,2,12],[3,2,2,3,3,4,28],[2,6,4,1,4,2,25],[6,0,2,1,3,3,11]]
[2,5,4,6,5,2]
*/
class Solution {
public:
    int encodeNeeds(vector<int>& needs) {
        int toRet = 0;
        for (int i = 0; i < needs.size(); ++i) {
            toRet *= BASE;
            toRet += needs[i];
        }
        return toRet;
    }
    
    int deduceEncodedNeeds(int* toDeduce, int n, int remainingNeeds) {
        int toRet = 0, b = 1;
        int copiedNeeds = remainingNeeds;
        for (int i = n-1; i >= 0; --i) {
            int localNeed = copiedNeeds%BASE;
            if (localNeed < toDeduce[i]) return INVALID;
            localNeed -= toDeduce[i];
            toRet += localNeed*b;
            copiedNeeds /= BASE;
            b *= BASE;
        }
                
        if (debug) {
            printf("Deducing [");
            for (int i = 0; i < n; ++i) printf("%d%s", toDeduce[i], (i == n-1 ? "" : ", "));
            printf("] from remainingNeeds:%d results in %d\n", remainingNeeds, toRet);
        }
        return toRet;
    }
    
    int dfs(vector<int>& price, vector<vector<int>>& special, int remainingNeeds) {
        if (INVALID != cache[remainingNeeds]) return cache[remainingNeeds];
        if (0 == remainingNeeds) return cache[0] = 0;
        int cand = INT_MAX;
        
        int toDeduce[6] = {0, 0, 0, 0, 0, 0};
        int n = price.size();
        for (int i = 0; i < n; ++i) {
            toDeduce[i] = 1;
            int nextRemainingNeeds = deduceEncodedNeeds(toDeduce, n, remainingNeeds);
            if (INVALID != nextRemainingNeeds) {
                int tmp = dfs(price, special, nextRemainingNeeds);
                if (INVALID != tmp) {
                    cand = min(cand, price[i]+tmp);
                }
            }
            toDeduce[i] = 0; // backtrace
        }
        
        for (int j = 0; j < special.size(); ++j) {
            memcpy(toDeduce, &special[j][0], n*sizeof(int));
            int nextRemainingNeeds = deduceEncodedNeeds(toDeduce, n, remainingNeeds);
            if (INVALID != nextRemainingNeeds) {
                int tmp = dfs(price, special, nextRemainingNeeds);
                if (INVALID != tmp) {
                    cand = min(cand, special[j][n]+tmp);
                }
            }
            // no need to backtrace "toDeduce" explicitly
        }
        if (debug) {
            printf("assigning %d to cache[remainingNeeds:%d]\n", cand, remainingNeeds);
        }
        return cache[remainingNeeds] = (INT_MAX == cand ? INVALID : cand);
    }
    
    int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
        memset(cache, INVALID, sizeof cache);
        int remainingNeeds = encodeNeeds(needs);
        if (debug) {
            printf("remainingNeeds is %d\n", remainingNeeds);
        }
        int ans = dfs(price, special, remainingNeeds);
        return ans;
    }
};
