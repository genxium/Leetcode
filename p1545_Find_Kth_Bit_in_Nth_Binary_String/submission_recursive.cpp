int const INTEGER_BIT_LENGTH = sizeof(int)*CHAR_BIT;

class Solution {
private:
    int intPow(int x, int n) {
        int nBinaryLengthUpper = 0;
        while (nBinaryLengthUpper < INTEGER_BIT_LENGTH && (1 << nBinaryLengthUpper) <= n) ++nBinaryLengthUpper;
            
        int ans = 1;
        /*
        * The ORDER OF i-TRAVERSAL is important here. Consider the following examples.
        *
        //////////////////////////////////////////////////////////////////
        * n:23 :(10111)b
        * traversal [1:x => 0:x^2 => 1:x^4*x => 1:x^10*x => 1:x^22*x]
        *
        //////////////////////////////////////////////////////////////////
        * n:10 :(1010)b
        * traversal [1:x => 0:x^2 => 1:x^4*x => 0:x^10]
        */
        for (int i = nBinaryLengthUpper-1; i >= 0; --i) {
            ans *= ans;
            if (((1 << i) & n) > 0) {
                ans *= x;
            }
        }
        return ans;
    }
     
public:
    int dfs(int n, int k) {
        // midPos = (pow(2, n) - 2)/2 = pow(2, n-1)-1
        if (n == 1) {
            return 0;
        }
        int midPos = intPow(2, n-1) - 1; 
        if (k == midPos) return 1;
        if (k < midPos) {
            return dfs(n-1, k);
        } else {
            int l = intPow(2, n) - 1;
            return !dfs(n-1, l - k - 1);
        }
    }
    
    char findKthBit(int n, int k) {
        /*
        * Denote l[n] being the length of S[n] (starting from n:1), it's obvious that "l[n+1] = 2*l[n]+1".
        * Therefore "(l[n+1]+1) = 2*(l[n]+1)" and "l[n] = pow(2, n) - 1".
        */
        --k;
        
        int ans = dfs(n, k);
        return ('0' + ans);
    }
};
