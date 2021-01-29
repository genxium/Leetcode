typedef unordered_set<string> ADJ_ELE;
typedef unordered_map<string, ADJ_ELE> ADJ_LIST;
int const INVALID = -1;

/*
test case#1
"AACCGGTT"
"AAACGGTA"
["AACCGGTA", "AACCGCTA", "AAACGGTA"]

test case#2
"AAAAACCC"
"AACCCCCC"
["AAAACCCC", "AAACCCCC", "AACCCCCC"]
*/
class Solution {
private:
  int distance(string &lhs, string &rhs) {
    if (lhs.length() != rhs.length()) return INVALID;
    
    int toRet = 0;
    for (int i = 0; i < lhs.length(); ++i) {
      if (lhs[i] != rhs[i]) ++toRet;
    }
    return toRet;
  }
  
  int dijkstra(string &start, string &end, ADJ_LIST &adj) {        
    // See https://www.yinxiang.com/everhub/note/8f531e5d-f4ab-4164-a459-266ab4b8cda8 for basics of the Dijkstra algorithm.
    priority_queue< pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>> > openHeap;
    set<string> closedSet;
    unordered_map<string, int> g; // "g[u] == x" being the shortest path to reach "u" from "start"

    // init
    openHeap.push({0, start});
    g[start] = 0;

    // loop
    while (!openHeap.empty()) {
      auto expander = openHeap.top(); openHeap.pop();
      string &u = expander.second;
      
      closedSet.insert(u);
      if (0 == u.compare(end)) {
        return g[u];
      }
      
      auto nbs = adj[u];
      for (auto &v : nbs) {
        if (closedSet.find(v) != closedSet.end()) continue;

        int candidate = g[u] + 1;
        if (g.find(v) == g.end() || candidate < g[v]) {
          g[v] = candidate;
          openHeap.push({candidate, v});
        }
      }
    }
    
    return INVALID;    
}

public:
  int minMutation(string start, string end, vector<string>& bank) {
    bool endIsValid = false;
    
    ADJ_LIST adj;
    for (auto it = bank.begin(); it != bank.end(); ) {
      if (0 == start.compare(*it)) {
        it = bank.erase(it);
      } else if (0 == end.compare(*it)) {
        endIsValid = true;
        ++it;
      } else {
        ++it;
      }
    }
    
    if (!endIsValid) {
      return INVALID;
    }
        
    for (auto &intermediate : bank) {
      int d1 = distance(start, intermediate);
      if (1 == d1) {
        adj[start].insert(intermediate);
      }
      
      int d2 = distance(intermediate, end);
      if (1 == d2) {
        adj[intermediate].insert(end);
      }
    }
    
    for (int i = 0; i < bank.size(); ++i) {
      for (int j = i+1; j < bank.size(); ++j) {
        int d = distance(bank[i], bank[j]);
        if (1 == d) {
          adj[bank[i]].insert(bank[j]);
          adj[bank[j]].insert(bank[i]);
        }
      }
    }
        
    int ans = dijkstra(start, end, adj);
    return ans;
  }
};
