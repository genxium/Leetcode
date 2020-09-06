#define MOD 1337
int INTEGER_BIT_LENGTH = sizeof(int)*CHAR_BIT;

class Solution {
public:
    int multiplyMod(int lhs, int rhs) {
        return (lhs*rhs)%MOD;
    }
    
    int myPow(int x, int n) {
        // See https://github.com/genxium/Leetcode/tree/master/p50_Pow_x_n for an explanation of the custom integer power implementation.

        if (0 == x) return x; 
        if (1 == x) return x;
        if (0 == n) return 1;
        if (1 == n) return x;
            
        int nBinaryLengthUpper = 0;
        while (nBinaryLengthUpper < INTEGER_BIT_LENGTH && (1 << nBinaryLengthUpper) <= n) ++nBinaryLengthUpper;
        
        int ans = 1;
        for (int i = nBinaryLengthUpper-1; i >= 0; --i) {
            ans = multiplyMod(ans, ans);
            if (((1 << i) & n) > 0) {
                ans = multiplyMod(ans, x);
            }
        }
        
        return ans;
    }

    int superPow(int a, vector<int>& b) {
        /*
        e.g. 
        
        a^(t*10 + r) = (a^(t*10))*(a^r)
                     = ((a^t)^10)*(a^r)
        
        now that if "t = u*10 + v"
        
        a^(t*10 + r) = ((a^(u*10+v))^10)*(a^r)
                     = ((((a^u)^10)*(a^v))^10)*(a^r)
        */
        a = a%MOD;
        int ans = 1;
        for (int i = 0; i < b.size(); ++i) {
            ans = multiplyMod(ans, myPow(a, b[i]));
            if (i < b.size()-1) {
                ans = myPow(ans, 10);
            }
        }
        return ans;
    }
};
