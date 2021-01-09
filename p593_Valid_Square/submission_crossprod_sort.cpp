bool const debug = false;

typedef vector<int> VI;
int p0[2];

bool cmp(VI const& lhs, VI const& rhs) {
    int a[] = {lhs[0]-p0[0], lhs[1]-p0[1]};
    int b[] = {rhs[0]-p0[0], rhs[1]-p0[1]};
    
    return (a[0]*b[1]-a[1]*b[0] > 0); // "cross-prod(a, b) > 0" means that "lhs" turns counterclockwisely to "rhs"
}

/*
test cases
[0,0]
[1,1]
[1,0]
[0,1]
[0,0]
[1,1]
[1,0]
[0,12]
[1,0]
[-1,0]
[0,1]
[0,-1]
*/
class Solution {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4) {
        p0[0] = p1[0]; p0[1] = p1[1];
        
        vector<VI> pts = {p1, p2, p3, p4};
        sort(pts.begin(), pts.end(), &cmp);
        
        int a[] = {pts[1][0] - pts[0][0], pts[1][1] - pts[0][1]};
        int b[] = {pts[2][0] - pts[1][0], pts[2][1] - pts[1][1]};
        int c[] = {pts[3][0] - pts[2][0], pts[3][1] - pts[2][1]};
        int d[] = {pts[0][0] - pts[3][0], pts[0][1] - pts[3][1]};
        
        if (debug) {
            printf("sorted\n");
            for (auto &pt : pts) {
                printf("{%d, %d}\n", pt[0], pt[1]);
            }
            printf("a: {%d, %d}\n", a[0], a[1]);
            printf("b: {%d, %d}\n", b[0], b[1]);
            printf("c: {%d, %d}\n", c[0], c[1]);
            printf("d: {%d, %d}\n", d[0], d[1]);
        }
        // dot-prod(a, b)
        if (0 != a[0]*b[0]+a[1]*b[1]) return false;
        
        // dot-prod(b, c)
        if (0 != b[0]*c[0]+b[1]*c[1]) return false;
        
        // now that a perp b, b perp c, thus a//c is implied
        if (
            a[0]*a[0]+a[1]*a[1] != c[0]*c[0]+c[1]*c[1]
            ||
            b[0]*b[0]+b[1]*b[1] != d[0]*d[0]+d[1]*d[1]
            ||
            a[0]*a[0]+a[1]*a[1] != b[0]*b[0]+b[1]*b[1]
            ||
            0 == a[0]*a[0]+a[1]*a[1]
        ) return false;
        
        return true;
    }
};
