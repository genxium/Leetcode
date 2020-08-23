/*
Compared with "https://github.com/genxium/Leetcode/blob/master/p1462_Course_Schedule_IV/submission_recursive_reverse_adj_list_TLE.cpp", the current approach avoids many slow "STL uses", and replaces the "discrete heapmem `list<int>`" with "continuous heapmem `vector<int>`" as much as possible.
*/
#define INVALID -1
#define MAXN 100
bool visited[MAXN];

class Solution {
private:
    bool predecessors[MAXN][MAXN]; // "predecessors[v][u] == true" means that "v" has "u" as a predecessor

    void updatePredecessorsByRevAdj(int u, int n, vector<int> &successorsByFar, unordered_map<int, vector<int>> &revAdj) {
        if (visited[u]) {
            for (auto &vv : successorsByFar) {
                if (u != vv) {
                    predecessors[vv][u] = true;
                }
                for (int k = 0; k < n; ++k) {
                    if (predecessors[u][k]) {
                        predecessors[vv][k] = true;                        
                    }
                }
            }
            return;
        }
        
        for (auto &vv : successorsByFar) {
            if (u != vv) {
                predecessors[vv][u] = true;
                //predecessors[vv].insert(u);      
            }
        }
        
        successorsByFar.push_back(u);
        auto revNbs = revAdj[u];
        for (auto &revNb : revNbs) {
            updatePredecessorsByRevAdj(revNb, n, successorsByFar, revAdj);
        }
        visited[u] = true;
        successorsByFar.pop_back();
    }

public:
    vector<bool> checkIfPrerequisite(int n, vector<vector<int>>& prerequisites, vector<vector<int>>& queries) {
        unordered_map<int, vector<int>> revAdj;
        for (auto &edge : prerequisites) {
            int u = edge[0];
            int v = edge[1];
            
            if (revAdj.find(v) == revAdj.end()) {
                revAdj[v] = {};
            }
            revAdj[v].push_back(u);
        }
    
        memset(predecessors, false, sizeof(predecessors));
        memset(visited, false, sizeof(visited));
        vector<int> successorsByFar;
        for (int v = 0; v < n; ++v) {
            successorsByFar.push_back(v);
            updatePredecessorsByRevAdj(v, n, successorsByFar, revAdj);
            successorsByFar.pop_back();
        }
        
        vector<bool> ans;
        for (auto &query : queries) {
            ans.push_back(predecessors[query[1]][query[0]]);
        }
        
        return ans;
    }
};
