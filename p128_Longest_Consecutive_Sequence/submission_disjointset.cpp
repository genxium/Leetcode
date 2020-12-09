unordered_map<int, int> djsParent; // "DisjointSetParent"
unordered_map<int, int> rtSize; // setSizeOf(djsFindRoot(num))

bool const debug = false;

int djsFindRoot(int u) {
    int cur = u;
    if (djsParent.find(cur) == djsParent.end()) {
        djsParent[u] = u; // lazy init
        rtSize[u] = 1;
        return u;
    }
    int p = djsParent[cur];
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

    if (uRoot <= vRoot) {
        rtSize[uRoot] = rtSize[uRoot] + rtSize[vRoot]; 
        djsParent[vRoot] = uRoot;
        return uRoot;
    } else {
        // vRoot < uRoot
        rtSize[vRoot] = rtSize[vRoot] + rtSize[uRoot]; 
        djsParent[uRoot] = vRoot;
        return vRoot;
    }
}

/*
test cases
[100,4,200,1,3,2]
[0,3,7,2,5,8,4,6,0,1]
[]
[4,0,-4,-2,2,5,2,0,-8,-8,-8,-8,-1,7,4,5,5,-4,6,6,-3]
*/
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        djsParent.clear();
        int ans = 0;
        for (auto &num : nums) {
            int rtNum = djsFindRoot(num);
            if (rtNum != num) continue; // already taken
            if (djsParent.find(num-1) != djsParent.end()) {
                djsUnite(num-1, num);
            }
            if (djsParent.find(num+1) != djsParent.end() && djsFindRoot(num+1) == num+1) {
                djsUnite(num, num+1);
            }
            rtNum = djsFindRoot(num);
            if (rtSize[rtNum] > ans) {
                if (debug) printf("Updating ans by num:%d, where rtSize[rtNum:%d] is %d\n", num, rtNum, rtSize[rtNum]);
                ans = rtSize[rtNum];
            }
        }
        return ans;
    }
};
