bool debug = false;
int const INVALID = -1;
int const MAXTARGET = 10000;
int const MAXPOS = 16384; // such "(1 << m)-1 >= MAXTARGET" 

int cache[MAXPOS];

/*
test cases
3
4
5
6
230
43
10000
*/
class Solution {
public:
    int dfs(int t) {
        if (0 == t) return cache[t] = 0;
        if (INVALID != cache[t]) return cache[t];

        int m = 0;
        while ((1 << m) <= t) {
            ++m;
        }

        // Way #1
        int far = (1 << m) - 1;
        int cand1 = (far == t ? m : m+1+dfs(far-t));
        if (INVALID == cache[t] || cand1 < cache[t]) cache[t] = cand1;

        // Way #2
        for (int p = 0; p < (m-1); ++p) {
            int near = (1 << (m-1)) - (1 << p); // In fact ((1 << (m-1)) - 1) - ((1 << (p)) - 1)
            int cand2 = (m-1+p+2) +dfs(t-near); // Even if "p == 0", we needed the 2 turn arounds to reset "speed = 1".
            if (INVALID == cache[t] || cand2 < cache[t]) cache[t] = cand2;    
        }

        return cache[t];
    }

    int racecar(int target) {
        /*
        A consecutive A's from "abs(speed) == 1" will result in "abs(PositionDiff) == pow2(k)-1" where k is the number of A's in the sequence. 
        
        A general "t: (1...10...01...10...0)" where count of all digits is "m" can be reached from "pos: 0, speed: 1" in two different ways
        - go to "far: (1...11...11...11...1), speed: pow2(m)" then turn around and the problem is reduced to solve "newPos: 0, newSpeed: 1, newTarget: (far-t)", or 
        - go to "near: (1...10...0), speed: 1" where the count of all digits is EXACTLY "m-1", thus it needs 2 turn arounds, i.e. one in the middle and one at the end, then the problem is reduced to solve "newPos: 0, newSpeed: 1, newTarget: (t-near)".

        Note that it"s a strong claim that we DON'T NEED traverse all "x < t" for "near" -- which is also important to avoid TLE since "target can be as large as 10000". There's NO RIGOROUS PROOF by the time of writing.
        */

        memset(cache, INVALID, sizeof cache);
        for (int t = 0; t < target; ++t) {
            // [TODO] In fact, since each "cache[(1 << m)-1]" has been prepared by far, we can do this iteratively.
            dfs(t);
        }

        return dfs(target);
    }
};
