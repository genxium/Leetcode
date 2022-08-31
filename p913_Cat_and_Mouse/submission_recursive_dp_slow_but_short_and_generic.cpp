bool const debug = false;
int const HOLE = 0;
int const MAXN = 50;
int const WINNING = 1, LOSING = 2, INVALID = -1;   
int const MOUSE_TURN = 0, CAT_TURN = 1;
typedef vector<int> VI;

/*
A "StateTuple" is {mousePos, catPos, mouseOrCatTurn}.
*/
int dp[MAXN][MAXN][2]; // could only be one of {WINNING, LOSING, INVALID}
vector<VI> stAdj[MAXN][MAXN][2]; // the "StateTupleGraph", each element is {adjMousePos, adjCatPos, adjMouseOrCatTurn}
int undeterminedNbCount[MAXN][MAXN][2]; // [WARNING] Refers to the "neighbour of StateTupleGraph" instead of the "OriginalGraph".
bool openSet[MAXN][MAXN][2]; // a.k.a. "onStack[MAXN][MAXN][2]"
bool closedSet[MAXN][MAXN][2]; // a.k.a. "wasOnButNowOffStack[MAXN][MAXN][2]"

/*
test cases
[[2,5],[3],[0,4,5],[1,4,5],[2,3],[0,2,3]]
[[1,3],[0],[3],[0,2]]
[[5,6],[3,4],[6],[1,4,5],[1,3,5],[0,3,4,6],[0,2,5]]
```
6 - 2
| \
0 - 5 - 4
    | / |
    3 - 1
```
*/
class Solution {
public:
    void bottomUpDfs(int mousePos, int catPos, int turn, vector<VI> &graph, int level) {  
        if (closedSet[mousePos][catPos][turn]) {
            /*
            [WARNING] Don't re-enter a determined "StateTuple" by "bottomUpDfs", or the current implementation will result in incorrect decrements "undeterminedNbCount" for the "neighbours of StateTupleGraph".
            */
            return;
        }
        int indent = 2*level;
        openSet[mousePos][catPos][turn] = true;
        for (auto& prevState: stAdj[mousePos][catPos][turn]) {
            int prevMousePos = prevState[0], prevCatPos = prevState[1], prevTurn = prevState[2]; 
            if (openSet[prevMousePos][prevCatPos][prevTurn]) continue;
            if (INVALID != dp[prevMousePos][prevCatPos][prevTurn]) {
                // Already propagated when it was just determined.
                continue;
            }
            if (LOSING == dp[mousePos][catPos][turn]) {
                undeterminedNbCount[prevMousePos][prevCatPos][prevTurn] = 0;
                dp[prevMousePos][prevCatPos][prevTurn] = WINNING;
                if (debug) printf("%*s#1,dp[prevMousePos:%d][prevCatPos:%d][prevTurn:%d] is just determined to be WINNING, by dp[mousePos:%d][catPos:%d][turn:%d] being LOSING\n", indent, "", prevMousePos, prevCatPos, prevTurn, mousePos, catPos, turn);
            } else if (--undeterminedNbCount[prevMousePos][prevCatPos][prevTurn] == 0) {
                // Must be "WINNING == dp[mousePos][catPos][turn]" by now because we ignored "INVALID == dp[mousePos][catPos][turn]" cases
                dp[prevMousePos][prevCatPos][prevTurn] = LOSING;
                if (debug) printf("%*s#2,dp[prevMousePos:%d][prevCatPos:%d][prevTurn:%d] is just determined to be LOSING by depletion\n", indent, "", prevMousePos, prevCatPos, prevTurn);                    
            }

            if (INVALID == dp[prevMousePos][prevCatPos][prevTurn]) {
                // Only propagates from determined "StateTuple"s.
                continue;
            }

            bottomUpDfs(prevMousePos, prevCatPos, prevTurn, graph, level+1);
        }
        
        openSet[mousePos][catPos][turn] = false;     
        closedSet[mousePos][catPos][turn] = true;     
    }
    
    int catMouseGame(vector<vector<int>>& graph) {
        int n = graph.size();
        
        // init
        memset(dp, INVALID, sizeof dp);
        memset(undeterminedNbCount, INVALID, sizeof undeterminedNbCount);
        memset(openSet, false, sizeof openSet);
        memset(closedSet, false, sizeof closedSet);
                
        for (int mousePos = 0; mousePos < n; ++mousePos) {
            for (int catPos = 0; catPos < n; ++catPos) {
                if (HOLE == catPos || HOLE == mousePos) {
                    // Whoever is in the hole, the mouse wins.
                    undeterminedNbCount[mousePos][catPos][MOUSE_TURN] = 0;
                    undeterminedNbCount[mousePos][catPos][CAT_TURN] = 0;
                    dp[mousePos][catPos][MOUSE_TURN] = WINNING;
                    dp[mousePos][catPos][CAT_TURN] = LOSING;
                } else if (catPos == mousePos) {
                    // However the cat meets the mouse, the cat wins
                    undeterminedNbCount[mousePos][catPos][MOUSE_TURN] = 0;
                    undeterminedNbCount[mousePos][catPos][CAT_TURN] = 0;
                    dp[mousePos][catPos][MOUSE_TURN] = LOSING;
                    dp[mousePos][catPos][CAT_TURN] = WINNING;
                } else {
                    undeterminedNbCount[mousePos][catPos][MOUSE_TURN] = graph[mousePos].size();
                    undeterminedNbCount[mousePos][catPos][CAT_TURN] = graph[catPos].size();
                }
                
                // build stAdj
                stAdj[mousePos][catPos][MOUSE_TURN].clear();
                stAdj[mousePos][catPos][CAT_TURN].clear();
                
                // if now mouse turn, previous cat turn
                for (int adjCatPos : graph[catPos]) {
                    stAdj[mousePos][catPos][MOUSE_TURN].push_back({mousePos, adjCatPos, CAT_TURN});
                }
                // if now cat turn, previous mouse turn
                for (int adjMousePos : graph[mousePos]) {
                    stAdj[mousePos][catPos][CAT_TURN].push_back({adjMousePos, catPos, MOUSE_TURN});
                }
            }
        }
        

        
        // loop
        /*
        The dfs must be "bottom-up", i.e. from determined "StateTuple"s to undetermined ones. Consider the following example to see the difficulty of the "top-down" approach, where "u1, u2, u3, u4, v" are all undetermined "StateTuple"s but "ds" is "determined & can determine u1".
        
        u1 - u2 - u3 - u4
        |\         \   |
        | \         \  | 
        ds \         \ | 
            \----------v
            
        A "top-down" dfs 
        - starting from "StateTuple u1" might go "u1 -> u2 -> u3 -> u4 -> v" to reach "StateTuple v" first, and then "v" would have no "not-in-openset/not-on-stack StateTupleNeighbour" to go on, thus stays undetermined; 
        - the dfs stack then pops back to "u4", also having no "not-in-openset/not-on-stack StateTupleNeighbour" to go on; 
        - the dfs stack then pops back to "u3" and goes on to "v" again, now that "u4" is a "not-in-openset/not-on-stack StateTupleNeighbour" of "v", shall we check it? yes but it will immediately return to "v", remaining undetermined too;
        - ...
        - the dfs stack then pops back to "u1" and goes on to "v" again, now that both "u3" and "u4" are "not-in-openset/not-on-stack StateTupleNeighbours" of "v", shall we check them? yes but again they'll immediately return to "v", remaining undetermined too;
        - the dfs stack then pops back to "u1" and goes on to "ds", which now makes "u1" determined too.
        
        It's only by now that we can do a "top-down" dfs from "u1" again to determine all remaining "StateTuple"s, which merely resembles the "bottom-up" approach!
        */
        
        for (int mousePos = 0; mousePos < n; ++mousePos) {
            for (int catPos = 0; catPos < n; ++catPos) {
                for (int turn = 0; turn < 2; ++turn) {
                    if (INVALID == dp[mousePos][catPos][turn]) continue;
                    bottomUpDfs(mousePos, catPos, turn, graph, 0);
                }
            }
        }
    
        return INVALID == dp[1][2][MOUSE_TURN] ? 0 : dp[1][2][MOUSE_TURN];
    }
};

