bool debug = false;
typedef uint64_t ULL;
int const MAXVAL = 1000000000;
int const MAXN = 100000;

int maskedA[MAXN], maskedB[MAXN];

class Solution {
public:
    int getXORSum(vector<int>& arr1, vector<int>& arr2) {
        int n1 = arr1.size(), n2 = arr2.size();
        
        int highestSingleSetbitMask = 1 << (int)log2(MAXVAL);
        
        int ans = 0;
        
        for (int k = 0; (1 << k) <= highestSingleSetbitMask; ++k) {
            // Calculate the k-th bit of the ans.
            
            int maskForThisBit = (1 << k);
            
            ULL bOneCnt = 0, oneCntForThisBit = 0;
            for (int i = 0; i < n2; ++i) {
                maskedB[i] = (arr2[i] & maskForThisBit) ? 1 : 0;
                bOneCnt += maskedB[i];
            }
            if (debug) printf("k:%d, bOneCnt is %d\n", k, bOneCnt);
            
            for (int i = 0; i < n1; ++i) {
                maskedA[i] = (arr1[i] & maskForThisBit) ? 1 : 0;
                if (0 == maskedA[i]) continue;
                oneCntForThisBit += bOneCnt;
            }
            
            if (debug) printf("k:%d, oneCntForThisBit is %d\n", k, oneCntForThisBit);
            
            if (oneCntForThisBit % 2 == 1) ans += maskForThisBit;
        }
        
        return ans;
    }
};
