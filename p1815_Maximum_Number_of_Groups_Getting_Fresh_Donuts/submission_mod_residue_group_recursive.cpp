bool debug = true;
typedef uint64_t ULL;

ULL pow30[9];

/*
test cases
3
[1,2,3,4,5,6]
4
[1,3,2,5,2,2,1,6]
6
[369205928,981877451,947462486,899465743,737778942,573732515,520226542,824581298,571789442,251943251,70139785,778962318,43379662,90924712,142825931,182207697,178834435,978165687]
6
[427131612,605667633,430658688,184870663,657607733,95828484,250750555,521834986,8028559,190673048,567689172,340635609,537836866,958889391,696686201,988310133,470639242,998695730,737137861,686240045,785358340,242376365,255952863]
*/
class Solution {
public:
    ULL encode(vector<int> &residueDict) {
        // The max of encoding, i.e. 30*(1 + 30 + ... + 30^8), is about 30^9 and less than 2^64.
        ULL key = 0;
        int batchSize = residueDict.size();
        for (int intrinsicResidue = 0; intrinsicResidue < batchSize; ++intrinsicResidue) {
            key += residueDict[intrinsicResidue]*pow30[intrinsicResidue];
        }

        return key;
    }

    int dfs(int preResidue, vector<int> &residueDict, ULL key, int i, int n, vector<unordered_map<ULL, int>> &cache) {
        if (i == n) return 0;
        if (cache[preResidue].count(key)) return cache[preResidue][key];
        int ret = 0;
        if (preResidue == 0) {
            ++ret;
        }
        int inc = 0, batchSize = residueDict.size();
        for (int intrinsicResidue = 0; intrinsicResidue < batchSize; ++intrinsicResidue) {
            auto &cnt = residueDict[intrinsicResidue];
            if (cnt <= 0) continue;
            --cnt;
            ULL nextKey = key - pow30[intrinsicResidue];
            int nextResidue = (preResidue-intrinsicResidue+batchSize)%batchSize;
            inc = max(inc, dfs(nextResidue, residueDict, nextKey, i+1, n, cache));
            ++cnt;
        }
       
        return cache[preResidue][key] = ret+inc;
    }

    int maxHappyGroups(int batchSize, vector<int>& groups) {
        memset(pow30, 0, sizeof pow30);
        pow30[0] = 1;
        for (int i = 1; i < 9; ++i) {
            pow30[i] = 30u*pow30[i-1];
        }
        
        int n = groups.size();
        /*
        As n is up to 30, we couldn't traverse either "all permutations (30!)" or "all subsets (2^30)" to find the answer, there must be somewhere to cut search space.

        Note that for this problem, we can call "(previousResidue, remainingGroups)" a state, and traverse each group in "remainingGroups" for an answer, however per the concern above this bruteforce approach will exceeed time limit. 

        The trick is that for any "previousResidue", each group that has the same "intrinsicResidue (w.r.t. batchSize)" is EQUIVALENT, i.e. we don't have to traverse all groups, but just 1 group per different "intrinsicResidue".

        The rest of the trick is straight forward: encoding the "state" and caching.   
        */

        vector<unordered_map<ULL, int>> cache(batchSize);
        vector<int> residueDict(batchSize); 
        for (auto &group : groups) {
            int intrinsicResidue = group%batchSize;
            ++residueDict[intrinsicResidue];
        }

        ULL key = encode(residueDict);
        return dfs(0, residueDict, key, 0, n, cache);
    }
};
