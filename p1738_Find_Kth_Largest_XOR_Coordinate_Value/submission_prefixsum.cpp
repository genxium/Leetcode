bool const debug = false;
int const MAXY = 1000, MAXX = 1000;
int const INVALID = -1;
int pX[MAXY][MAXX]; // prefixXor

/*
test cases
[[5,2],[1,6]]
1
[[5,2],[1,6]]
2
[[5,2],[1,6]]
3
[[5,2],[1,6]]
4
*/
class Solution {
public:
    int kthLargestValue(vector<vector<int>>& matrix, int k) {
        int yUpper = matrix.size(), xUpper = matrix[0].size();
        
        // init 
        memset(pX, INVALID, sizeof pX);
        pX[0][0] = matrix[0][0];
        vector<int> candidates = {pX[0][0]};
        
        // loop
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                if (INVALID != pX[y][x]) continue;
                pX[y][x] = matrix[y][x];
                
                if (y > 0 && x > 0) {
                    int tmp = pX[y-1][x]^pX[y][x-1]^pX[y-1][x-1];
                    pX[y][x] ^= tmp;
                } else if (y > 0) {
                    int tmp = pX[y-1][x];
                    pX[y][x] ^= tmp;
                } else if (x > 0) {
                    int tmp = pX[y][x-1];
                    pX[y][x] ^= tmp;
                }
                
                candidates.push_back(pX[y][x]);
            }
        }
        
        if (debug) {
            for (auto c : candidates) printf("%d ", c);
            puts("");
        }
        --k;
        nth_element(candidates.begin(), candidates.begin()+k, candidates.end(), greater<int>());
        return *(candidates.begin()+k);
    }
};
