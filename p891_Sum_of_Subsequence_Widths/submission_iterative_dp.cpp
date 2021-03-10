bool debug = true;
typedef pair<int, int> PII;
typedef uint64_t ULL;
ULL const ULLMAX = UINT64_MAX;
ULL const MOD = 1000000007u;

int const MAXN = 20000;
ULL pow2[MAXN+1];

/*
test cases
[3,1,2]
[3,1]
[1,2,3,4]
[3,7,2,3]
[10,4,2,10,9,3,2]
*/
class Solution {
public:
    int sumSubseqWidths(vector<int>& nums) {
        pow2[0] = 1;
        for (int x = 1; x <= MAXN; ++x) {
            pow2[x] = (pow2[x-1] << 1);
            pow2[x] %= MOD;
        }

        map<int, int> counter;
        for (auto num : nums) ++counter[num];

        vector<PII> items;
        for (auto &it : counter) {
            items.push_back({it.first, it.second});
        }

        /*
        For each "items[i]", traverse all "j < i", then each nonempty-subset using "items[j].val" as the smallest can add "1 ~ items[i].cnt" of "items[i].val" to make a new nonempty-subset whose smallest is still "items[j].val".

        foreach (i) {
            #subsetWithSmallest(items[i]) := pow2(items[i].cnt)-1 // using "1 ~ items[i].cnt" of "items[i].val" to make new subsets, NOTE THAT "same value but different index is deemed "different"!
            foreach (j < i) {
                ans += SUM{#subsetWithSmallest(items[i])*(items[i].val - items[j].val)*#subsetWithSmallest(items[j])}
                #subsetWithSmallest(items[j]) *= (#subsetWithSmallest(items[i])+1) // adding "0 ~ items[i].cnt" to each subset to make a new subset
            }
        }

        Note that "#subsetWithSmallest(...)" counts ONLY nonempty-subsets.

        A naive nested-forloop implementation will result in TLE, thus a little math insight is needed to speed up, as shown below.
        */

        int n = items.size();
        ULL ans = 0;
    
        ULL acc1 = (pow2[items[0].second]-1); // This variable represents "SUM{#subsetWithSmallest(items[j])}" for each "i".
        ULL acc2 = items[0].first*(pow2[items[0].second]-1); // This variable represents "SUM{items[j].val*#subsetWithSmallest(items[j])}" for each "i".
        for (int i = 1; i < n; ++i) {
            ULL countAllSubsetsForThisVal = pow2[items[i].second]; 
            ULL countNonemptySubsetsForThisVal = countAllSubsetsForThisVal-1; 
            
            ans += countNonemptySubsetsForThisVal*(acc1*items[i].first - acc2 + MOD)%MOD;
            ans %= MOD;

            acc1 *= countAllSubsetsForThisVal; acc1 %= MOD;
            acc1 += countNonemptySubsetsForThisVal; acc1 %= MOD;

            acc2 *= countAllSubsetsForThisVal; acc2 %= MOD;
            acc2 += countNonemptySubsetsForThisVal*items[i].first%MOD; acc2 %= MOD;
        }

        return ans;
    }
};
