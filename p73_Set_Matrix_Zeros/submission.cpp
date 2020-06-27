unordered_set<int> mockZeroCandidates;
int MOCK_ZERO;

class Solution {
public:
    Solution() {
        mockZeroCandidates.insert(INT_MAX);
        mockZeroCandidates.insert(INT_MIN);
        for (int i = 0; i < 10; ++i) {
            mockZeroCandidates.insert(INT_MAX-(i << 1));
            mockZeroCandidates.insert(INT_MIN+(i << 1));    
        }
    }
    
    void setZeroes(vector<vector<int>>& matrix) {
        int yUpper = matrix.size();
        if (0 == yUpper) return;
        int xUpper = matrix[0].size();
        
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                mockZeroCandidates.erase(matrix[y][x]);
            }
        }
        MOCK_ZERO = *(mockZeroCandidates.begin());
        
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                if (matrix[y][x] == 0) {
                    for (int i = 0; i < y; ++i) {
                        matrix[i][x] = 0;
                    }
                    for (int j = 0; j < x; ++j) {
                        matrix[y][j] = 0;
                    }
                    for (int i = y+1; i < yUpper; ++i) {
                        if (matrix[i][x] != 0) {
                            matrix[i][x] = MOCK_ZERO;
                        }
                    }
                    for (int j = x+1; j < xUpper; ++j) {
                        if (matrix[y][j] != 0) {
                            matrix[y][j] = MOCK_ZERO;   
                        }
                    }
                } else if (matrix[y][x] == MOCK_ZERO) { 
                    matrix[y][x] = 0;
                } else {
                    // Do nothing.
                }
            }
        }
    }
};
