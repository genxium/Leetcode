bool const debug = false;
typedef vector<int> VI;
typedef unordered_set<int> NBS;
typedef unordered_map<int, NBS> ADJ;

/*
test cases
8
2
[-1,-1,1,0,0,1,0,-1]
[[],[6],[5],[6],[3,6],[],[],[]]
8
2
[-1,-1,1,0,0,1,0,-1]
[[],[6],[5],[6],[3],[],[4],[]]
4
1
[-1,0,0,-1]
[[],[0],[1,3],[2]]
*/
class Solution {
public:
    vector<int> topsort(ADJ &adj) {
        // Topological sorting of a DirectedGraph, returns an array whose size is less than adj.size() if a ring exists; otherwise return an array such that if "u can reach v", then "u precedes v in the returned array".
        unordered_map<int, int> inDegree;
        for (auto &[u, nbs] : adj) {
            for (auto v : nbs) {
                ++inDegree[v];
            }
        }

        queue<int> q; // Use a queue to do it iteratively.
        vector<int> toRet;
        
        // init
        for (auto &[u, nbs] : adj) {
            if (inDegree[u]) continue;
            q.push(u);
        }

        // loop
        while (!q.empty()) {
            auto u = q.front(); q.pop();
            toRet.push_back(u);
            for (auto v : adj[u]) {
                --inDegree[v];
                if (0 != inDegree[v]) continue;
                q.push(v);
            }
        }

        return toRet;
    }

    vector<int> sortItems(int n, int m, vector<int>& group, vector<vector<int>>& beforeItems) {
        ADJ adjItems, adjGroups, groupToItems; 

        int extendedM = m;
        for (int u = 0; u < n; ++u) {
            if (-1 == group[u]) {
                group[u] = extendedM++;
            }
            groupToItems[group[u]].insert(u);
        }
        if (debug) printf("groupToItems.size() == %d\n", groupToItems.size());

        for (int v = 0; v < n; ++v) {
            // init each
            if (adjGroups[group[v]].empty()) adjGroups[group[v]] = {}; 
            if (adjItems[v].empty()) adjItems[v] = {};

            // assign each 
            for (auto u : beforeItems[v]) {
                adjItems[u].insert(v);
                if (group[u] == group[v]) continue;
                adjGroups[group[u]].insert(group[v]);
            }
        }

        vector<int> ans;
        if (debug) printf("adjGroups.size() == %d\n", adjGroups.size());
        vector<int> orderedGroupIdList = topsort(adjGroups);
        if (debug) {
            printf("orderedGroupIdList is:");
            for (auto groupId : orderedGroupIdList) {
                printf(" %d", groupId);
            } puts("");
        }
        // If there's a cycle in "adjGroups", we cannot produce a valid answer.
        if (orderedGroupIdList.size() < adjGroups.size()) {
            return {};
        }

        for (int groupId : orderedGroupIdList) {
            NBS intraGroupItems = groupToItems[groupId];
            ADJ intraGroupAdjItems;
            for (auto u : intraGroupItems) {
                if (intraGroupAdjItems[u].empty()) intraGroupAdjItems[u] = {};
                for (auto v : adjItems[u]) {
                    if (!intraGroupItems.count(v)) continue;
                    intraGroupAdjItems[u].insert(v);
                }
            }
            vector<int> orderedIntraGroupItemList = topsort(intraGroupAdjItems);
            if (orderedIntraGroupItemList.size() < intraGroupAdjItems.size()) {
                return {};
            }
            if (debug) {
                printf("orderedIntraGroupItemList[groupId:%d] is:", groupId);
                for (auto u : orderedIntraGroupItemList) {
                    printf(" %d", u);
                } puts("");
            }
            for (auto u : orderedIntraGroupItemList) ans.push_back(u);
        }

        return ans;
    }
};
