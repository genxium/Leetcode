typedef vector<int> VI;
int const DISJOINT_SET_NOT_FOUND = -1;
int const MAXN = 100000;
int djsParent[MAXN+1]; // "DisjointSetParent"
bool const debug = false;

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

bool qCmp(VI const &lhs, VI const &rhs) {
    return lhs[2] < rhs[2];
}

bool eCmp(VI const &lhs, VI const &rhs) {
    return lhs[2] < rhs[2];
}

class Solution {
public:
    vector<bool> distanceLimitedPathsExist(int n, vector<vector<int>>& edgeList, vector<vector<int>>& queries) {
        vector<VI> reorderedQs;
        for (int i = 0; i < queries.size(); ++i) {
            VI &query = queries[i];
            reorderedQs.push_back({query[0], query[1], query[2], i});
        }
        sort(reorderedQs.begin(), reorderedQs.end(), &qCmp);
        sort(edgeList.begin(), edgeList.end(), &eCmp);

        vector<bool> ans(reorderedQs.size(), false);
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof(djsParent));
        int edgeIdx = 0;
        for (auto &query : reorderedQs) {
            int lmt = query[2];
            for (; edgeIdx < edgeList.size() && edgeList[edgeIdx][2] < lmt; ++edgeIdx) {
                int u = edgeList[edgeIdx][0], v = edgeList[edgeIdx][1];
                int uRoot = djsFindRoot(u), vRoot = djsFindRoot(v);
                djsUnite(uRoot, vRoot);
            }
            int p = query[0], q = query[1];
            int pRoot = djsFindRoot(p), qRoot = djsFindRoot(q);
            if (pRoot == qRoot) {
                ans[query[3]] = true;
            }
        }

        return ans;
    }
};
