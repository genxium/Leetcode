bool const debug = false;
int const MAXN = 100000, DISJOINT_SET_NOT_FOUND = -1;

int djsParent[MAXN]; // "DisjointSetParent"

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
    if (uRoot < vRoot) {
        return (djsParent[vRoot] = uRoot);        
    } else {
        return (djsParent[uRoot] = vRoot);
    }
}

class Solution {
public:
    int minHammingOfPositions(vector<int>& source, vector<int>& target, vector<int>& posList) {
        unordered_map<int, int> extractedSrc;
        vector<int> extractedTgt;
        
        if (debug) printf("extracting posList\n");
        for (auto &pos : posList) {
            if (debug) printf(" source[%d]:%d, target[%d]:%d", pos, source[pos], pos, target[pos]);
            ++extractedSrc[source[pos]];
            extractedTgt.push_back(target[pos]);
        }
        
        if (debug) {
            printf("\n\textracted\n");
        }
        
        // We can permute "extractedSrc" arbitrarily if "any amount of swaps" is allowed.
        
        int toRet = 0;
        for (int i = 0; i < posList.size(); ++i) {
            if (0 == extractedSrc[extractedTgt[i]]) ++toRet;
            else --extractedSrc[extractedTgt[i]];
        }
        
        return toRet;
    }
    
    int minimumHammingDistance(vector<int>& source, vector<int>& target, vector<vector<int>>& allowedSwaps) {
        int n = source.size();
        
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof djsParent);
        for (auto &allowedSwap : allowedSwaps) {
            djsUnite(allowedSwap[0], allowedSwap[1]);
        }
        
        unordered_map<int, vector<int>> counter; // djsParent -> posList that shares this djsParent
        for (int i = 0; i < n; ++i) {
            int rt = djsFindRoot(i);
            counter[rt].push_back(i);
        }
        
        int ans = 0;
        for (auto &it : counter) {
            ans += minHammingOfPositions(source, target, it.second);
        } 
        
        return ans;
    }
};
