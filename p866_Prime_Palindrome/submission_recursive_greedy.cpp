bool debug = false;
typedef uint64_t ULL;
int const INVALID = -1;

/*
test cases
1
6
8
13
192
100000000
*/
class Solution {
public:
    bool isPrime(int x) {
        // There's a faster deterministic version of prime test, which is a variant of "Miller-Rabin algorithm".
        if (1 == x) return false;
        int sq = (int)ceil(sqrt(x));
        for (int d = 2; d <= sq; d++) {
            if (x % d == 0) return false;
        }
        return true;
    }
    
    int genPrimePalindrome(int len /* expected length */, int i, string &buff, int N) {
        for (int d = 0; d <= 9; ++d) {
            if (i == 0) {
                if (d == 0) continue; // don't start with leading 0, and no prime will end with an even digit
            }
            buff.push_back('0'+d);

            if ((len & 1) && i == (len >> 1)) {
                // The expected length of root is odd, and we reaches half
                string u = buff;
                string v = buff; v.pop_back(); reverse(v.begin(), v.end());
                u.append(v);
                int x = stoi(u);
                if (isPrime(x)) {
                    if (debug) printf("%d is a palindrome prime\n", x);
                    if (x >= N) return x;
                }
            } else if (!(len & 1) && i+1 == (len >> 1)) {
                // The expected length of root is even, and we reaches half
                string u = buff;
                string v = buff; reverse(v.begin(), v.end());
                u.append(v);
                int x = stoi(u);
                if (isPrime(x)) {
                    if (debug) printf("%d is a palindrome prime\n", x);
                    if (x >= N) return x;
                }
            } else {
                int x = genPrimePalindrome(len, i+1, buff, N);
                if (INVALID != x) return x;
            }

            buff.pop_back(); // backtrace
        }
        
        return INVALID;
    }

    int primePalindrome(int N) {
        if (N < 10) {
            int smaillPrimes[] = {2, 3, 5, 7, 11};
            auto it = lower_bound(smaillPrimes, smaillPrimes+5, N);
            return *it;
        }
        string u = to_string(N);
        int lenSt = u.length(), lenEd = 9;
        for (int l = lenSt; l <= lenEd; ++l) {
            string buff = "";
            int x = genPrimePalindrome(l, 0, buff, N);   
            if (INVALID != x) return x;
        }
        
        return INVALID;
    }
};
