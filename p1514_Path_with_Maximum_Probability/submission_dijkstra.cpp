class Solution {
public:
    double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start, int end) {
        unordered_map<int, vector<pair<int, double>>> adj;
        int E = edges.size();
        for (int i = 0; i < E; ++i) {
            int u = edges[i][0];
            int v = edges[i][1];
            double w = succProb[i];
        
            if (adj.find(u) == adj.end()) {
                adj[u] = vector<pair<int, double>>{};
            } 
            adj[u].push_back({v, w});
          
            if (adj.find(v) == adj.end()) {
                adj[v] = vector<pair<int, double>>{};
            }
            adj[v].push_back({u, w});
        }
        unordered_map<int, double> g; // "g[u] == p" being the largest probability to reach "u" from start
        priority_queue< pair<double, int>, vector<pair<double, int>>, less<pair<double, int>> > openHeap;
        set<int> closedSet;

        // See https://www.yinxiang.com/everhub/note/8f531e5d-f4ab-4164-a459-266ab4b8cda8 for basics of the Dijkstra algorithm.
        
        // init
        openHeap.push({1.0, start});
        g[start] = 1.0;
        
        // loop
        while (false == openHeap.empty()) {
            auto expander = openHeap.top(); openHeap.pop();
            int u = expander.second;
            double prop = expander.first;
            //printf("checking expander (prop: %lf, u: %d)\n", prop, u);
            closedSet.insert(u);
            auto nbs = adj[u];
            for (auto &nb : nbs) {
                int v = nb.first;
                double w = nb.second;
                if (closedSet.find(v) != closedSet.end()) continue;
                //printf("\tchecking nb (w: %lf, v: %d)\n", w, v);

                double candidate = w*prop;
                if (g.find(v) == g.end() || g[v] < candidate) {
                    g[v] = candidate;
                    //printf("\tpushing (candidate: %lf, v: %d) to openHeap\n", candidate, v);
                    openHeap.push({candidate, v});
                }
            }
        }
        return g[end];
    }
};
