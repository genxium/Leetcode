class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int yUpper = matrix.size();
        int xUpper = matrix[0].size();
        vector<int> flattened;
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                flattened.push_back(matrix[y][x]);
            }
        }
        nth_element(flattened.begin(), flattened.begin()+k-1, flattened.end());
        int ans = flattened[k-1];
        return ans;
    }
};
