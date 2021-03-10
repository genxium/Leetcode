class Solution {
public:
    int gcd(int x, int y) {
        if (y == 0) return x;
        return gcd(y, x%y);
    }

    int mirrorReflection(int p, int q) {
        int d = gcd(p, q);
        int t = p/d;
        int v = p*t, u = q*t;
        int m = v/p, n = u/p;

        // Imagine an infinite grid of squares WITHOUT reflection, the problem is equivalent to that the ray will finally reach (m*p, n*p).

        if (m%2 == 0) {
            // mirror exits "1" and "2" horizontally
            if (n%2 == 0) return 1;
            else return 2;
        } else {
            if (n%2 == 0) return 0;
            else return 1;
        }
    }
};
