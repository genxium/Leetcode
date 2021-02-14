int const MAXN = 1000000;
bool isPrime[MAXN+1];

void primeSieve(int upper) {
    int sq = (int)ceil(sqrt(upper));
    memset(isPrime, true, sizeof isPrime);
    isPrime[0] = false;
    isPrime[1] = false;
    for (int p = 2; p <= sq; ++p) {
        if (!isPrime[p]) continue;
        for (int j = 2; j*p <= upper; ++j) {
            isPrime[j*p] = false;
        }
    }
}

/*
test cases
244
269
842
888
*/
class Solution {
public:
    int countPrimeSetBits(int L, int R) {
        primeSieve((int)(log2(R+1)));
        int ans = 0;
        for (int i = L; i <= R; ++i) {
            if (isPrime[__builtin_popcount(i)]) {
                ++ans;
            }
        }
        return ans;
    }
};
