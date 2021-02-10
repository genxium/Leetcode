bool debug = false;

/*
test cases
[1,2,1,2,3]
2
[1,2,1,3,4]
3
[1,2]
1
*/
class Solution {
public:
    int subarrayCount(vector<int> &A, int atMostX) {
        int toRet = 0;
        if (0 == atMostX) return 0;
        
        int n = A.size(), l = 0, r = 0;
        unordered_map<int, int> counter;

        int round = 0;
        while (r < n) {
            if (debug) printf("round:%d, l = %d, r = %d, toRet = %d\n", round, l, r, toRet);
            while (r < n) {
                if (counter.size() == atMostX && false == counter.count(A[r])) break;
                ++counter[A[r]];
                ++r; // Keep "r" being the open-end    
                toRet += (r-l); // For each new "r", every [u, r) forms a subarray with "<= atMostX" distinct elements where "u >= l"
            }

            if (debug) printf("\tmid, l = %d, r = %d, toRet = %d\n", l, r, toRet);

            while (l < r && counter.size() >= atMostX) {
                --counter[A[l]];
                if (0 == counter[A[l]]) counter.erase(A[l]); 
                ++l;
            }
            
            // For every "v = the `l` just moved", [v, r' > r) will contain more than "atMostX" distinct elements, thus no need to move "r" by now.

            if (debug) printf("\tend, l = %d, r = %d, toRet = %d\n", l, r, toRet);

            ++round;
        }

        return toRet;
    }
    
    int subarraysWithKDistinct(vector<int>& A, int K) {
        int atMostKCnt = subarrayCount(A, K);
        if (debug) puts("");
        int atMostKMinus1Cnt = subarrayCount(A, K-1);
        int ans = atMostKCnt-atMostKMinus1Cnt;
        return ans;
    }
};
