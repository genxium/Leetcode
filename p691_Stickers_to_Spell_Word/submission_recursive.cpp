bool const debug = false;
int const INVALID = -1;
int const INTEGER_BIT_LENGTH = sizeof(int)*CHAR_BIT;
int const MAXCH = 128;
int const MAXN = 50;
typedef uint64_t LL;

int contriIdcs[MAXCH]; // "indics of contribution"
vector<unordered_map<LL, int>> cache; // "cache[i][remainingState] == x" means that using "contriCntsMap[i, ...]" to make up "remainingState" needs at least "x" stickers 

/*
As "target" has length ranged in [1, 15], each letter contribution should be [0, 15], however being a concatenation of two randomly chosen words, "target" is nearly impossible to have a single character repeated 8 times.
*/
int const BASE = 8;
int const BINARY_OFFSET_MULTIPLIER = log2(BASE);
int const ESTIMATED_MAX_OF_CONTRIIDX = 20;
LL cachePows[ESTIMATED_MAX_OF_CONTRIIDX];

class Solution {
public:
    LL encodeState(unordered_map<char, int> &letterCnts) {
        /*
        * e.g. 
        * letterCnts {a: 5, d: 2, g: 7, c: 0, h: 5, s: 0, y: 7}
        * contriIdcs  0     1     2     3     4     5     6
        */
        LL toRet = 0;
        for (auto [letter, v] : letterCnts) {
            toRet += v*cachePows[contriIdcs[letter]];
        }
        return toRet;
    }

    int dfs(int i, vector<unordered_map<char, int>> &contriCntsMap, LL remainingState) {
        if (0 == remainingState) return 0;
        if (i == contriCntsMap.size()) return INT_MAX;
        if (cache[i].count(remainingState)) return cache[i][remainingState];
        auto &contriCnts = contriCntsMap[i];

        if (debug) {
                printf("Checking contriCnts ");
                for (auto it : contriCnts) {
                    printf("(%c:%d) ", it.first, it.second);
                } printf("\nfor remainingState:%d\n", remainingState);
        }

        int times = 1, toRet = INT_MAX;
        // If doesn't skip the current "contriState"
        for (int times = 1; ; ++times) {
            bool hasFurtherReduced = false;
            LL newRemainingState = remainingState;
            for (auto &[letter, contri] : contriCnts) {   
                int contriIdx = contriIdcs[letter];    
                int mask = cachePows[contriIdx+1] - cachePows[contriIdx];
                int rem = ((remainingState&mask) >> (contriIdx*BINARY_OFFSET_MULTIPLIER));
                /*
                // It's a trick that produces equivalent result as the following statement, but only works when BASE is a power of 2.
                int rem = (remainingState%cachePows[contriIdx+1])/cachePows[contriIdx];

                For example, 
                ```
                letter          t,   h,   e,   a
                contriIdx       3,   2,   1,   0
                remaining       2,   2,   1,   1
                base2         010, 010, 001, 001
                base10                1161
                ```

                To extract the remaining of "h", we shall 
                - take "s = remainingState&(000,111,000,000) = remainingState&((000,111,111,111)-(000,000,111,111))"
                - then take "t = s/(1,000,000)" being the result we want.
                */
                if (debug) printf("\tremaining of letter %c is %d, contribution to it is %d\n", letter, rem, contri);
                if (0 == rem) {
                    continue;
                }
                int tmp1 = contri*(times-1);
                if (contri > 0 && rem - tmp1 > 0) {
                    hasFurtherReduced = true;
                }
                int tmp2 = (tmp1+contri);
                // newRemainingState -= ((rem < tmp2 ? rem : tmp2) * cachePows[contriIdx]);
                newRemainingState -= ((rem < tmp2 ? rem : tmp2) << (contriIdx*BINARY_OFFSET_MULTIPLIER));
            }
            if (false == hasFurtherReduced) break;
            if (debug) printf("\ttimes == %d, newRemainingState is %d\n", times, newRemainingState);
            int cand = dfs(i+1, contriCntsMap, newRemainingState);
            if (INT_MAX != cand) {
                toRet = toRet < times+cand ? toRet : times+cand;
            }
        }

        // If just skips the current "contriState"
        int cand = dfs(i+1, contriCntsMap, remainingState);
        toRet = min(toRet, cand);

        return cache[i][remainingState] = toRet;
    }

    int minStickers(vector<string>& stickers, string target) {
        cachePows[0] = 1;
        for (int i = 1; i < 16; ++i) {
            cachePows[i] = cachePows[i-1]*BASE;
        }

        cache.clear();
        cache.assign(stickers.size(), {});

        memset(contriIdcs, INVALID, sizeof contriIdcs);

        unordered_map<char, int> remaining;
        for (auto ch : target) {
            ++remaining[ch];
        }

        int contriIdx = 0;
        for (auto it : remaining) {
            contriIdcs[it.first] = contriIdx++;
            if (debug) printf("contriIdcs[%c] == %d\n", it.first, contriIdcs[it.first]);
        }

        vector<unordered_map<char, int>> contriCntsMap; // "contriCntsMap[i]" is the contribution to "target"
        for (int j = 0; j < stickers.size(); ++j) {
            unordered_map<char, int> contriCnts;
            for (auto ch : stickers[j]) {
                if (!remaining.count(ch)) continue;
                if (contriCnts[ch] >= remaining[ch]) continue;
                ++contriCnts[ch];
            }
            if (0 == contriCnts.size()) continue;
            contriCntsMap.push_back(contriCnts);
            if (debug) {
                printf("Got new contriCnts\n");
                for (auto it : contriCnts) {
                    printf("(%c:%d) ", it.first, it.second);
                } puts("");
            }
        }

        LL remainingState = encodeState(remaining);
        int ans = dfs(0, contriCntsMap, remainingState);
        if (INT_MAX == ans) return INVALID;
        return ans;
    }
};
