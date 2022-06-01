typedef vector<int> VI;
bool const debug = false;
/*
test cases
[[1,1],[1,3],[3,1],[3,3],[2,2]]
[[1,1],[1,3],[3,1],[3,3],[4,1],[4,3]]
[[3,2],[3,1],[4,4],[1,1],[4,3],[0,3],[0,2],[4,0]]
[[0,0],[2,1],[2,4],[2,3],[1,4],[3,4],[1,1],[4,0]]
*/
class Solution {
public:
    int minAreaRect(vector<vector<int>>& points) {
        sort(points.begin(), points.end());
        int n = points.size();
        int ans = INT_MAX;
        auto cmp = less<VI>();
        for (int i = 0; i < n-3; ++i) {
            auto& p1 = points[i];
            for (int j = i+1; j < n; ++j) {
                auto& p4 = points[j];
                if (p1[0] == p4[0] || p1[1] == p4[1]) continue;
                auto st = points.begin()+i+1, ed = points.begin()+j; 
                auto ip2 = lower_bound(st, ed, VI{p1[0], p4[1]}, cmp);
                auto p2 = *ip2;
                if (ip2 == ed) continue;
                if (p2[0] != p1[0] || p2[1] != p4[1]) continue;
                auto ip3 = lower_bound(st, ed, VI{p4[0], p1[1]}, cmp);
                if (ip3 == ed) continue;
                auto p3 = *ip3;
                if (p3[0] != p4[0] || p3[1] != p1[1]) continue;
                if (debug) {
                    printf("For i:%d, p1 = {%d, %d}; j:%d, p4 = {%d, %d}\n", i, p1[0], p1[1], j, p4[0], p4[1]);
                    printf("\tfound p2 = {%d, %d}; p3 = {%d, %d}\n", p2[0], p2[1], p3[0], p3[1]);
                }
                ans = min(ans, (p4[1]-p1[1])*(p4[0]-p1[0]));
                if (j+1 < n && points[j+1][0] >= points[j][0] && points[j+1][1] >= points[j][1]) break;
            }
        }
        return INT_MAX == ans ? 0 : ans;
    }
};
