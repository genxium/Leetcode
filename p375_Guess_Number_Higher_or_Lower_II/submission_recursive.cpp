#define MAXN 201
#define INVALID -1
int cache[MAXN][MAXN]; // "cache[l][r] == x" means that the minimum amount of money to prepare for guessing between [l, r] while guaranteeing a win

class Solution {
public:
    int solve(int l, int r) {
        if (INVALID != cache[l][r]) {
            return cache[l][r];
        }
        if (l == r) {
            return (cache[l][r] = 0);
        }
        int result = INT_MAX;
        for (int guess = l; guess <= r; ++guess) {
            // if the correct number were on left-side
            int candidateLeft = INVALID;
            if (guess-1 >= l) {
                candidateLeft = solve(l, guess-1);
            }
            
            // if the correct number were on right-side
            int candidateRight = INVALID;
            if (guess+1 <= r) {
                candidateRight = solve(guess+1, r);
            }
            int candidateWhole = guess + max(candidateLeft, candidateRight);
            result = min(result, candidateWhole);
        }
        return cache[l][r] = result;
    }
    
    int getMoneyAmount(int n) {
        memset(cache, INVALID, sizeof(cache));
        int l = 1, r = n;
        int ans = solve(1, n);
        return ans;
    }
};
