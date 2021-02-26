bool debug = true;
double const INVALID = -1.0;
int const MAXN = 5000; // The answer is monotonically increasing w.r.t. "N", and about 5000 it can be rounded to "1.0".

double cache[MAXN][MAXN];
int da[] = {100, 75, 50, 25};
int db[] = {0, 25, 50, 75};

class Solution {
public:
    double soupServings(int N) {
        if (N >= MAXN) {
            return 1.0;
        }

        for (int i = 0; i <= N; ++i) {
            for (int j = 0; j <= N; ++j) {
                cache[i][j] = INVALID;
            }
        }
        
        return dfs(N, N);
    }
    
    double dfs(int A, int B) {
        if (A <= 0 && B <= 0) return 0.5;
        if (A <= 0) return 1.0;
        if (B <= 0) return 0.0;
        if (INVALID != cache[A][B]) {
            return cache[A][B];
        }
        cache[A][B] = 0.0;
        for (int i = 0; i < 4; i++) {
            cache[A][B] += dfs(A-da[i], B-db[i]);
        }
        cache[A][B] *= 0.25;
        return cache[A][B];
    }
};
