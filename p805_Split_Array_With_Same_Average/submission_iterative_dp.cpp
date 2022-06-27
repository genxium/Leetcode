bool const debug = false;
int const MAXN = 30;
int const MAXSINGLE = 10000;
int const MAXSUM = MAXN*MAXSINGLE;

/*
test cases
[1,2,3,4,5,6,7,8]
[1,2,3,4,5,6,7]
[1,100]
[1,6,1]
[0]
[3,1]
[5,16,4,11,4]
*/
class Solution {
public:
    bool dpBySumAsState(vector<int> &A) {
        // If "(dp[s] & (1 << k)) > 0", then we can make subset sum "s" by "k" elements from "A[]".
        int dp[MAXSUM+1]; 
        memset(dp, 0, sizeof dp);
        dp[0] = 1; // By definition, "(dp[0] & (1 << k)) > 0" should be true for (only) "k == 0".

        /*
        For a subset sized "k" and summed "s", within a wholeset sized "n" and summed "w", the result is true iff (s/k == w/n), i.e. (n*s == w*k) for the sake of accuracy.
        */
        int n = A.size(), w = 0;
        for (int i = 0; i < n; ++i) {
            w += A[i];
            // [WARNING] Deliberately reversed the order of traversing "s" here.
            for (int s = MAXSUM; s >= 0; --s) {
                if (0 == dp[s]) continue;
                dp[s + A[i]] |= (dp[s] << 1);
            }
        }

        for (int s = MAXSUM; s >= 0; --s) {
            // Neither "B" nor "C" could be empty set, so start with "k > 0".
            for (int k = 1; k < n; ++k) {
                if ((dp[s] & (1 << k)) > 0) {
                    if (n*s == w*k) {
                        if (debug) printf("Can produce subset sum %d by %d elements.\n", s, k);
                        return true;
                    }
                }
            }
        }

        return false;
    }
    
    bool dpByEleAsState(vector<int>& A) {
        /*
        The key idea here is to find the target subset in one of the following three sources.
        1. Find solely in the first half, i.e. A[0, ..., halfN).
        2. Find solely in the second half, i.e. A[halfN, ..., n).
        3. Find a "primer subset" with sum "s2" in the second half, then find the "counterpart subset" with sum "s1" in the first half. 
        */
        int n = A.size(), halfN = (n+1)/2, w = 0;
        for (int i = 0; i < n; ++i) {
            w += A[i];
        }
        
        unordered_map<int, unordered_set<int>> counterFirstHalf; // counterFirstHalf[k] == {sum1, sum2, ...}
        set<pair<int, int>> dp1; // any {k1, s1} in dp1 means that sum "s1" is possible using "k1" elements in the first half
        dp1.insert({0, 0}); // init
        for (int i = 0; i < halfN; ++i) {
            set<pair<int, int>> tmp; 
            for (auto &[k1, s1] : dp1) {
                tmp.insert({k1+1, s1+A[i]});
            }
            for (auto &[k1, s1] : tmp) {
                dp1.insert({k1, s1});
            }
        }
        
        for (auto &[k1, s1] : dp1) {
            if (k1 > 0 && n*s1 == w*k1) {
                if (debug) printf("Can produce subset sum %d by %d elements.\n", s1, k1);
                return true;
            }
            counterFirstHalf[k1].insert(s1);
        }
        
        set<pair<int, int>> dp2; // any {k2, s2} in dp2 means that sum "s2" is possible using "k2" elements in the second half
        dp2.insert({0, 0}); // init
        for (int i = halfN; i < n; ++i) {
            set<pair<int, int>> tmp; 
            for (auto &[k2, s2] : dp2) {
                tmp.insert({k2+1, s2+A[i]});
            }
            for (auto &[k2, s2] : tmp) {
                dp2.insert({k2, s2});
            }
        }
        
        for (auto &[k2, s2] : dp2) {
            if (k2 > 0 && n*s2 == w*k2) {
                if (debug) printf("Can produce subset sum %d by %d elements.\n", s2, k2);
                return true;
            }
            for (int k = k2+1; k < n; ++k) {
                if (w*k % n != 0) continue;
                int k1 = k-k2; // implied "k1 > 0", a non-empty counterpart
                int s = w*k/n;
                int s1 = s-s2;
                if (counterFirstHalf[k1].count(s1)) {
                    if (debug) printf("Can produce subset sum %d by %d + %d elements.\n", s, k1+k2);
                    return true;
                }
            }   
        }
        
        return false;   
    }
    
    bool dpByEleAsStateBitOp(vector<int>& A) {
        int n = A.size(), halfN = (n+1)/2, w = 0;
        for (int i = 0; i < n; ++i) {
            w += A[i];
        }
        
        int firstHalfStates = (1 << halfN), secondHalfStates = (1 << (n-halfN));
        if (debug) printf("n == %d, w == %d, firstHalfStates == %d, secondHalfStates == %d\n", n, w, firstHalfStates, secondHalfStates);
        unordered_map<int, unordered_set<int>> counterFirstHalf; // counterFirstHalf[k] == {sum1, sum2, ...}
        vector<int> dp1(firstHalfStates, 0); // dp1[10110] == A[1]+A[2]+A[4]
        for (int x = 0; x < firstHalfStates; ++x) {
            int lsb = (x & -x); // least significant bit
            if (0 == lsb) continue;
            int i = (int)log2(lsb);
            dp1[x] = dp1[x^lsb] + A[i]; // it's guaranteed that "(x^lsb) < x" here
            if (debug) printf("dp1[x:%d] == %d\n", x, dp1[x]);
            int k1 = __builtin_popcount(x);
            if (n*dp1[x] == w*k1) {
                if (debug) printf("Can produce subset sum %d by %d elements.\n", x, k1);
                return true;
            }
            counterFirstHalf[k1].insert(dp1[x]);
        }
        
        vector<int> dp2(secondHalfStates, 0); // dp2[10110] == A[1+halfN]+A[2+halfN]+A[4+halfN]
        for (int x = 0; x < secondHalfStates; ++x) {
            int lsb = (x & -x);
            if (0 == lsb) continue;
            int i = (int)log2(lsb);
            dp2[x] = dp2[x^lsb] + A[i+halfN];
            if (debug) printf("dp2[x:%d] == %d\n", x, dp2[x]);
            int k2 = __builtin_popcount(x);
            if (n*dp2[x] == w*k2) {
                if (debug) printf("Can produce subset sum %d by %d elements.\n", x, k2);
                return true;
            }
            
            for (int k = k2+1; k < n; ++k) {
                if (w*k % n != 0) continue;
                int k1 = k-k2; // implied "k1 > 0", a non-empty counterpart
                int s = w*k/n;
                int s2 = dp2[x], s1 = s-s2;
                if (counterFirstHalf[k1].count(s1)) {
                    if (debug) printf("Can produce subset sum %d by %d+%d elements.\n", x, k1+k2);
                    return true;
                }
            }
        }
        
        return false;   
    }
    
    bool splitArraySameAverage(vector<int>& A) {
        if (2 > A.size()) return false;
        // return dpBySumAsState(A);
        // return dpByEleAsStateBitOp(A);
        return dpByEleAsState(A); // slower than the "BitOp" version
    }
};
