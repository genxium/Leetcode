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
        unordered_map<int, set<int>> ptDict;
        for (auto& pt : points) {
            ptDict[pt[0]].insert(pt[1]);
        }
        
        vector<VI> segs;
        for (auto& p1 : points) {
            for (auto& p2y : ptDict[p1[0]]) {
                if (p2y <= p1[1]) continue;
                segs.push_back({p1[1], p2y-p1[1], p1[0]});
            }
        }

        unordered_map<int, unordered_map<int, VI>> segDict;
        for (auto& seg : segs) {
            segDict[seg[0]][seg[1]].push_back(seg[2]);
        }
        int ans = INT_MAX;
        for (auto& [y, subDict] : segDict) {
            for (auto &[h, xs] : subDict) {
                if (0 == xs.size()) continue;
                sort(xs.begin(), xs.end());
                for (int i = 0; i < xs.size()-1; ++i) {
                    ans = min(ans, h*(xs[i+1]-xs[i]));
                }
            }
        }
        if (INT_MAX == ans) return 0;
        return ans;
    }
};
