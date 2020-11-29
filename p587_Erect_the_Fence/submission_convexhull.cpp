typedef vector<int> pt;
int const COLINEAR = 0;
int const CLOCKWISE = -1;
int const COUNTER_CLOCKWISE = +1;

int orientation(pt &a, pt &b, pt &c) {
    int val = a[0]*(b[1]-c[1])+b[0]*(c[1]-a[1])+c[0]*(a[1]-b[1]);
    if (0 == val) return COLINEAR;
    return val/abs(val);
}

bool cmp(pt const &a, pt const &b) {
    return a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]);
}

vector<pt> convexHull(vector<pt>& points) {
    vector<pt> toRet;
    int n = points.size();
    if (1 == n) {
        toRet.push_back(points[0]);
        return toRet;
    }
    
    sort(points.begin(), points.end(), &cmp);
    pt p1 = points[0], p2 = points.back();
    vector<pt> up, down; // both are stacks
    up.push_back(p1);
    down.push_back(p1);
    for (int i = 1; i < n; i++) {
        pt &p0 = points[i];
        if (i == n - 1 || COUNTER_CLOCKWISE != orientation(p1, p0, p2)) {
            while (up.size() >= 2 && COUNTER_CLOCKWISE == orientation(up[up.size()-2], up[up.size()-1], p0)) {
                /*
                [WARNING]
                In this specific problem, we don't pop COLINEAR intermediates.
                */
                up.pop_back();
            }
            up.push_back(points[i]);
        }
        if (i == n - 1 || CLOCKWISE != orientation(p1, p0, p2)) {
            while(down.size() >= 2 && CLOCKWISE == orientation(down[down.size()-2], down[down.size()-1], p0)) {
                down.pop_back();
            }
            down.push_back(points[i]);
        }
    }

    set<pt> dedup;
    for (int i = 0; i < (int)up.size(); i++) {
        dedup.insert(up[i]);
    }

    for (int i = down.size()-2; i > 0; i--) {
        dedup.insert(down[i]);
    }
    
    for (auto pt : dedup) {
        toRet.push_back(pt);
    }
    
    return toRet;
}

/*
test cases
[[1,1],[2,2],[2,0],[2,4],[3,3],[4,2]]
[[1,2],[2,2],[4,2]]
*/
class Solution {
public:
    vector<vector<int>> outerTrees(vector<vector<int>>& points) {
        vector<pt> ch = convexHull(points);
        return ch;
    }
};
