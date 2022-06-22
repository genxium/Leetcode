/*
test cases
[[1,2],[2,1],[1,0],[0,1]]
[[0,1],[2,1],[1,1],[1,0],[2,0]]
[[0,3],[1,2],[3,1],[1,3],[2,1]]
[[24420,16685],[20235,25520],[14540,20845],[20525,14500],[16876,24557],[24085,23720],[25427,18964],[21036,14573],[24420,23315],[22805,24760],[21547,25304],[16139,23952],[21360,14645],[24715,17120],[19765,25520],[19388,25491],[22340,25005],[25520,19765],[25365,21320],[23124,15443],[20845,14540],[24301,16532],[16685,24420],[25100,17875],[22125,25100],[15699,23468],[14592,21131],[25460,19155],[17837,25084],[23468,24301],[25460,20845],[18453,25304],[21131,14592],[22805,15240],[19475,25500],[15443,23124],[25355,21360],[15285,22880],[20000,25525],[24085,16280],[22163,25084],[22880,15285],[14916,22163],[16280,24085],[24875,17400],[22600,24875],[14573,21036],[25427,21036],[17120,24715],[25500,19475]]
*/
typedef vector<int> VI;

class Solution {
public:
    int gcd(int a, int b) {
        if (0 == b) return a;
        return gcd(b, a%b);
    }

    double minAreaFreeRect(vector<VI>& points) {
        map<VI, vector<VI>, less<VI>> segs; // {/gcd, /gcd, lengthsquared} -> segment left point
        sort(points.begin(), points.end(), less<VI>());
        int n = points.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i+1; j < n; ++j) {
                VI &p1 = points[i], &p2 = points[j];
                int dx = p2[0] - p1[0], dy = p2[1] - p1[1];
                int l2 = dx*dx + dy*dy;
                int g = gcd(max(abs(dx), abs(dy)), min(abs(dx), abs(dy)));
                dx /= g;
                dy /= g;
                if (dx*dy <0) {
                    // put negative at dx for indexing
                    if (dy < 0) {
                        dy *= -1;
                        dx *= -1;
                    }
                }
                VI id = {dx, dy, l2};
                segs[id].push_back(p1);
            }
        }

        double ans = DBL_MAX;
        for (auto &[id, plist] : segs) {
            for (int i = 0; i < plist.size(); ++i) {
                for (int j = i+1; j < plist.size(); ++j) {
                    VI &p1 = plist[i], &p3 = plist[j];
                    // p1, p3 and their associated segs form a parallelogram by now
                    int dx = p3[0] - p1[0], dy = p3[1] - p1[1];
                    int l2 = dx*dx + dy*dy;
                    int g = gcd(max(abs(dx), abs(dy)), min(abs(dx), abs(dy)));
                    dx /= g;
                    dy /= g;
                    if (-(dx*id[0]) == dy*id[1]) {
                        double area = sqrt(l2)*sqrt(id[2]);
                        ans = min(ans, area);
                    }     
                }
            }
        }
        if (DBL_MAX == ans) return 0.0;
        return ans;
    }
};
