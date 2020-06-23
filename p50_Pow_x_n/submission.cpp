#include <float.h>

double INF = std::numeric_limits<double>::infinity();
int INTEGER_BIT_LENGTH = sizeof(int)*CHAR_BIT;

class Solution {
public:
    double multiplyCapped(double lhs, double rhs) {
        // no negative number is concerned here
        if (DBL_MAX/rhs < lhs) return INF;
        return lhs*rhs;
    }
    
    int abs(int &n) {
        // No this is not a "faster approach", I'm just playing dirty.
        /*
        //////////////////////////////////////////////////
          +6     = (0 | 00...0110)
        + mask   = (0 | 00...0000)
        -----------------------------
                   (0 | 00..00110)
        ^ mask   = (0 | 00...0000)
        -----------------------------
                   (0 | 00..00110)
                   
        //////////////////////////////////////////////////
          -6     = (1 | 11...1010) // twos-complement of "+6", they add up to (0 | 00...0000)
        + mask   = (1 | 11...1111)
        -----------------------------
                   (1 | 11..11001)
        ^ mask   = (1 | 11...1111)
        -----------------------------
                   (0 | 00..00110)
        
        */ 
        int mask = (n >> (INTEGER_BIT_LENGTH - 1));
        /*
        Both approaches will overflow for
        - "INT_MIN + (-1)", i.e. "mask == (1 | 11...1111)b == (-1)d" due to that "(0 | 00...0001)b == (+1)d"
        , if n is not capped.
        
        // return (n ^ mask) - mask;
        */  
        return (n + mask)^mask; // This approach will overflow for 
    }
    
    bool isOdd(int &n) {
        return ((n&1) > 0);
    }
    
    double myPow(double x, int n) {
        /*
        test case#1
        0.00001
        2147483647
        
        test case#2
        2
        2147483647
        
        test case#3
        2
        -2147483648
        
        test case#4
        0.00001
        -2147483648
        */
        if (0.0f == x) return x; 
        if (1.0f == x) return x;
        if (0 == n) return 1.0f;
        if (1 == n) return x;
    
        int nn = n;
        bool needOneExtraMultiplication = false;
        if (nn == INT_MIN) {
            ++nn;
            needOneExtraMultiplication = true;
        }
        n = abs(nn);
        double sign = 1.0f;
        if (x < 0) {
            x = fabs(x);
            if (isOdd(n) && false == needOneExtraMultiplication) {
                sign = -1.0f;                
            }
        }
        //printf("abs(n:%d) == %d\n", nn, n);
        
        int nBinaryLengthUpper = 0;
        while (nBinaryLengthUpper < INTEGER_BIT_LENGTH && (1 << nBinaryLengthUpper) <= n) ++nBinaryLengthUpper;
            
        double ans = 1;
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
            ans = multiplyCapped(ans, ans);
            if (((1 << i) & n) > 0) {
                ans = multiplyCapped(ans, x);
            }
        }
        if (needOneExtraMultiplication) {
            ans = multiplyCapped(ans, x);
        }
        
        if (nn < 0) {
            //printf("inverting ans == %lf\n", ans);
            ans = 1.0f/ans;
        }
        return sign*ans;
    }
};
