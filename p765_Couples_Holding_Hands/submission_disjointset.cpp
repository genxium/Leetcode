bool const debug = false;

int const MAXN = 60;
int const OFFSET = 1024;

int const DISJOINT_SET_NOT_FOUND = -1;
int djsParent[MAXN+OFFSET]; // "DisjointSetParent"

int djsFindRoot(int u) {
    int cur = u;
    int p = djsParent[cur];
    if (DISJOINT_SET_NOT_FOUND == p) {
        djsParent[u] = u; // lazy init
        return u;
    }
    while (p != cur) {
        cur = p;
        p = djsFindRoot(cur); // lazy path compression
    }
    djsParent[u] = p;
    return p;
}
    
int djsUnite(int u, int v) {
    int uRoot = djsFindRoot(u);
    int vRoot = djsFindRoot(v);
    // Either "uRoot > vRoot" or "uRoot < vRoot" doesn't matter.
    return (djsParent[vRoot] = uRoot);
}

/*
test cases
[0, 2, 1, 3]
[3, 2, 0, 1]
[28,4,37,54,35,41,43,42,45,38,19,51,49,17,47,25,12,53,57,20,2,1,9,27,31,55,32,48,59,15,14,8,3,7,58,23,10,52,22,30,6,21,24,16,46,5,33,56,18,50,39,34,29,36,26,40,44,0,11,13]
*/
class Solution {
public:
    int minSwapsCouples(vector<int>& row) {
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof djsParent);
        int n = row.size();
        for (int i = 0; i < n; i+=2) {
            int groupIndex = i+OFFSET;
            if (debug) printf("groupIndx for i:%d is %d\n", i, groupIndex);
            djsUnite(row[i], groupIndex);
            djsUnite(row[i+1], groupIndex);
        }

        int ans = 0;
        for (int i = 0; i < n; i+=2) {
            int groupIndex1 = djsFindRoot(i);
            int groupIndex2 = djsFindRoot(i+1);
            if (groupIndex1 == groupIndex2) continue;

            /*
            Use 1 swap to put "i" and "i+1" into a same "group". It doesn't matter
            - whether we do "djsUnite(groupIndex1, groupIndex2)" or "djsUnite(groupIndex2, groupIndex1)", or
            - whether the elements currently paired with "i" and "i+1" are the same.

            Assume that the current pairs are "(i, x)" and "(i+1, y)", doing "djsUnite(groupIndex1, groupIndex2)" will make "(x, y)" in the same group as well.
            */
            djsUnite(groupIndex1, groupIndex2);
            ++ans; 
        }
        return ans;
    }
};
