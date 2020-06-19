class Solution {
public:
    pair<int, int> rotateNinety(int i, int j, int n) {
        // i(col scan, left->right) & j(row scan, top->down) are guaranteed to sit at the top-left block, and started from (0, 0) the top-left corner
        int k = (n >> 1);
        if (n%2 == 1) {
            return pair<int, int>{(k << 1)-j, i};
        } else {
            // In this case, assume that "k' = k - 0.5" is the virtual central index, the rotated result is again {2k'-j, i}
            return pair<int, int>{(k << 1)-j-1, i};
        }
    }
    
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        int k = (n >> 1);
        int iUpper, jUpper;
        if (n%2 == 1) {
            // top-left block is i:[0, k),j:[0, k), but we unite the i[0, k),j:[k] arm here.
            jUpper = k+1;
            iUpper = k;
            // [[1,2,3],[5,6,7],[8,9,10]]
            // [[1,2,3,4,5],[6,7,8,9,10],[11,12,13,14,15],[16,17,18,19,20],[21,22,23,24,25]]
        } else {
            // top-left block is i:[0, k), j:[0, k)
            jUpper = k;
            iUpper = k;
            // [[5,1,9,11],[2,4,8,10],[13,3,6,7],[15,14,12,16]]
        }
        for (int j = 0; j < jUpper; ++j) {
            for (int i = 0; i < iUpper; ++i) {
                int preVal = matrix[j][i];
                pair<int, int> rotatedCoor = rotateNinety(i, j, n);
                //printf("rotated (i:%d, j:%d) to (%d, %d)\n", i, j, rotatedCoor.first, rotatedCoor.second);
                while (!(rotatedCoor.first == i && rotatedCoor.second == j)) {
                    int ii = rotatedCoor.first;
                    int jj = rotatedCoor.second;
                    int tmp = matrix[jj][ii];
                    matrix[jj][ii] = preVal;
                    //printf("assigned matrix[jj:%d][ii:%d] = preVal:%d\n", jj, ii, preVal);

                    preVal = tmp;
                    rotatedCoor = rotateNinety(ii, jj, n);
                    //printf("rotated (i:%d, j:%d) to (%d, %d)\n", ii, jj, rotatedCoor.first, rotatedCoor.second);
                }
                matrix[j][i] = preVal;
                //printf("assigned matrix[j:%d][i:%d] = preVal:%d\n", j, i, preVal);
                //printf("\n");

            }
        }
    }
};
