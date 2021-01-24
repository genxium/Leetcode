bool const debug = false;
bool const printprimes = false;
typedef uint64_t ULL;
int const MAXN = 10000;
int const MAXK = 10000;
ULL const INVALID = 0u;
ULL const MOD = 1000000007u;

unordered_map<int, unordered_map<int, ULL>> C; // C[m][t] equals the number of ways to take t elements from p distinct elements, regardless of order

ULL combination(int p, int t) {
    if (p < 0 || t < 0) return 0;
    if (C.count(p) && C[p].count(t)) return C[p][t];
    if (t == p) return C[p][t] = 1u; // including {p: 0, t:0}
    if (t < p) {
        return C[p][t] = (combination(p-1, t-1) + combination(p-1, t))%MOD;
    }
    return INVALID;
}

bool isPrime[MAXN+1];
vector<int> primes;

void initPrimes() {
    memset(isPrime, true, sizeof(isPrime));

    isPrime[0] = false;
    isPrime[1] = false;
    for (int i = 2; i <= MAXN; ++i) {
        if (!isPrime[i]) continue;
        primes.push_back(i);
        for (int j = (i << 1); j < MAXN; j+=i) {
            isPrime[j] = false;
        }
    }
}

// Similar problem "p1569_Number_of_Ways_to_Reorder_Array_to_Get_Same_BST".
/*
test cases
[[2,6],[5,1],[73,660]]
[[2,2],[3,3],[5,5]]
[[1,1],[2,2],[3,3],[4,4],[5,5]]
*/
class Solution {
public:
    vector<int> waysToFillArray(vector<vector<int>>& queries) {
        initPrimes();
        if (debug && printprimes) {
            printf("primes are\n");
            for (auto p : primes) {
                printf("%d ", p);
            } puts("");
        }
        vector<int> ans;
        for (auto query : queries) {
            int n = query[0], k = query[1];
            if (debug) printf("n:%d, k:%d\n", n, k);
            
            ULL localAns = 1;
            
            for (int i = 0; i < primes.size() && primes[i] <= k; ++i) {
                // Assume that "k = PROD{pow(primes[i], q[i])}", For each "k%primes[i] == 0", figure out how many non-negative solutions exist for "x1 + x2 + ... + xn = q[i]", then take the product to be the "localAns". 
                int q = 0;
                while (k%primes[i] == 0) {
                    ++q;
                    k /= primes[i];
                }
                if (0 == q) continue;
                ULL countOfNonnegativeSols = combination(n+q-1, q);
                if (debug) printf("for p:%d, q is %d, countOfNonnegativeSols is %lu\n", primes[i], q, countOfNonnegativeSols);
                localAns *= countOfNonnegativeSols;
                localAns %= MOD;
            }
            
            ans.push_back((int)localAns);
        }
        
        return ans;
    }
};
