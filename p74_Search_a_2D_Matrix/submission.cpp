#define INVALID -1

class Solution {
public:
    int locateRow(vector<vector<int>>& matrix, int target) {
        int yUpper = matrix.size();
        if (0 == yUpper) return INVALID;
        int xUpper = matrix[0].size();
        if (0 == xUpper) return INVALID;
        int l = 0, r = yUpper;
        while (l <= r) {
            int mid = ((l + r) >> 1);
            int candidate = matrix[mid][0];
            //printf("l:%d, r:%d, mid: %d, target: %d, candidate: %d\n", l, r, mid, target, candidate);

            if (candidate == target) {
                return mid;
            } else if (candidate < target) {
                if (mid+1 < yUpper) {
                    int candidateNextRow = matrix[mid+1][0];
                    if (candidateNextRow == target) {
                        return mid+1;
                    } else if (candidateNextRow < target) {
                        l = mid+1;
                        continue;
                    } else {
                        // candidateNextRow > target
                        return mid;
                    }
                } else {
                    // mid == yUpper-1
                    return mid;
                }
            } else {
                // candidate > target
                r = mid-1;
                continue;
            }
        }
        return INVALID;
    }
    
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int targetRowIdx = locateRow(matrix, target);
        //printf("targetRowIdx == %d\n", targetRowIdx);
        if (INVALID == targetRowIdx) {
            return false;
        }
        vector<int> targetRow = matrix[targetRowIdx];
        auto it = lower_bound(targetRow.begin(), targetRow.end(), target);
        if (it == targetRow.end()) {
            return false;
        }
        if (*it != target) {
            return false;
        }
        return true;
    }
};
