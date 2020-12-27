typedef vector<int> VI;
bool cmp(VI const &lhs, VI const &rhs) {
    if (lhs[1] != rhs[1]) return lhs[1] < rhs[1]; // earlier end comes first
    return lhs[0] < rhs[0]; // earlier start comes first
}

/*
test cases
[[10,16],[2,8],[1,6],[7,12]]
[[1,2],[3,4],[5,6],[7,8]]
[[1,2],[2,3],[3,4],[4,5]]
[[1,2]]
[[2,3],[2,3]]
[[9,12],[1,10],[4,11],[8,12],[3,9],[6,9],[6,7]]
[[0,9],[1,8],[7,8],[1,6],[9,16],[7,13],[7,10],[6,11],[6,9],[9,13]]
*/
class Solution {
public:
    int findMinArrowShots(vector<vector<int>>& points) {
        sort(points.begin(), points.end(), &cmp);
        int n = points.size();
        int activeIdx = 0, ans = 0;
        for (int i = 1; i < n; ) {
            while (i < n && points[activeIdx][1] >= points[i][0]) {
                // in this case we can always shoot "activeIdx" to burst "i" together
                ++i;
            }
            activeIdx = i;
            ++ans;
            ++i;
        }
        
        // wrap up
        if (activeIdx == n-1) ++ans;
        
        return ans;
    }
};
