class Solution {
public:
    int matrixScore(vector<vector<int>>& A) {
        int nrows = A.size(), ncols = A[0].size();
        
        // First make all rows started by a "1".
        for (int r = 0; r < nrows; ++r) {
            if (A[r][0] == 0) {
                for (int c = 0; c < ncols; ++c) {
                    A[r][c] = !A[r][c];
                }
            }
        }
        
        int ans = nrows*(int)pow(2, ncols-1);
        
        // Use only column flipping from now on.
        for (int c = 1; c < ncols; ++c) {
            int oneCount = 0;
            for (int r = 0; r < nrows; ++r) {
                if (A[r][c] == 1) ++oneCount;
            }
            
            if (oneCount <= nrows/2) {
                oneCount = nrows-oneCount;
            }
            
            // Each "1" at this column contributes "pow(2, ncols-c-1)" to ans.
            ans += oneCount*(int)pow(2, ncols-c-1);
        }
        
        return ans;
    }
};
