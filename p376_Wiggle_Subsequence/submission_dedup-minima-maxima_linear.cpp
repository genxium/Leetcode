class Solution {
    public:
        int isLocalMinimaOrLocalMaxima(int index, vector<int>& dedupedNums) {
            int n = dedupedNums.size();
            if (0 <= index-1 && index+1 < n) {
                if (dedupedNums[index] > dedupedNums[index-1] && dedupedNums[index] > dedupedNums[index+1]) {
                    return +1; // maxima
                }
                if (dedupedNums[index] < dedupedNums[index-1] && dedupedNums[index] < dedupedNums[index+1]) {
                    return -1; // minima
                }
                return 0;
            } else if (index+1 < n) {
                if (dedupedNums[index] > dedupedNums[index+1]) {
                    return +1; // maxima
                }
                if (dedupedNums[index] < dedupedNums[index+1]) {
                    return -1; // minima
                }
                return 0;
            } else if (0 <= index-1) {
                if (dedupedNums[index] > dedupedNums[index-1]) {
                    return +1; // maxima
                }
                if (dedupedNums[index] < dedupedNums[index-1]) {
                    return -1; // minima
                }
                return 0;
            } else {
                return 0;
            }
        } 

        int wiggleMaxLength(vector<int>& nums) {
            if (0 == nums.size()) {
                return 0;
            }
            vector<int> dedupedNums;
            int curNum = nums[0];
            dedupedNums.push_back(curNum);
            for (int i = 1; i < nums.size(); ++i) {
                if (nums[i] != curNum) {
                    curNum = nums[i]; 
                    dedupedNums.push_back(curNum);
                } 
            }
            if (1 == dedupedNums.size()) {
                return 1;
            }

            vector<pair<int, int>> minimaMaxima;
            for (int i = 0; i < dedupedNums.size(); ++i) {
                int isMinimaMaxima = isLocalMinimaOrLocalMaxima(i, dedupedNums);
                if (0 == isMinimaMaxima) {
                    continue;
                }
                minimaMaxima.push_back({dedupedNums[i], isMinimaMaxima});
            }
            return minimaMaxima.size();
        }
};
