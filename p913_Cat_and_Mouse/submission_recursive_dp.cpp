bool const debug = false;
int const MAXN = 50;
int const MOUSE_WINS = 1, CAT_WINS = 2, DRAW = 0, INVALID = -1;   
int const MOUSE_TURN = 0, CAT_TURN = 1;
typedef vector<int> VI;

/*
A "StateTuple" is {mousePos, catPos, mouseOrCatTurn}.
*/
int dp[MAXN][MAXN][2];
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
    void bottomUpDfs(int mousePos, int catPos, int mouseOrCatTurn, vector<VI> &graph, int level) {  
        if (closedSet[mousePos][catPos][mouseOrCatTurn]) {
            /*
            [WARNING] Don't re-enter a determined "StateTuple" by "bottomUpDfs", or the current implementation will result in incorrect decrements "undeterminedNbCount" for the "neighbours of StateTupleGraph".
            */
            return;
        }
        int indent = 2*level;
        openSet[mousePos][catPos][mouseOrCatTurn] = true;
        if (mouseOrCatTurn == CAT_TURN) {
            int prevTurn = !mouseOrCatTurn; // MOUSE_TURN
            for (auto prevMousePos : graph[mousePos]) {
                if (openSet[prevMousePos][catPos][prevTurn]) continue;
                if (INVALID != dp[prevMousePos][catPos][prevTurn]) {
                    // Already propagated when it was just determined.
                    continue;
                }
                if (dp[mousePos][catPos][mouseOrCatTurn] == MOUSE_WINS) {
                    undeterminedNbCount[prevMousePos][catPos][prevTurn] = 0;
                    dp[prevMousePos][catPos][prevTurn] = MOUSE_WINS;
                    if (debug) {
                        printf("%*s#1,dp[mousePos:%d][catPos:%d][turn:%d] is just determined to be MOUSE_WINS, by dp[nextMousePos:%d][nextCatPos:%d][nextTurn:%d] being MOUSE_WINS\n", indent, "", prevMousePos, catPos, prevTurn, mousePos, catPos, mouseOrCatTurn);
                    }
                } else {
                    --undeterminedNbCount[prevMousePos][catPos][prevTurn];
                    if (debug) {
                        printf("%*sundeterminedNbCount[mousePos:%d][catPos:%d][turn:%d] is decremented to %d by dp[nextMousePos:%d][nextCatPos:%d][nextTurn:%d] being CAT_WINS\n", indent, "", prevMousePos, catPos, prevTurn, undeterminedNbCount[prevMousePos][catPos][prevTurn], mousePos, catPos, mouseOrCatTurn);
                    }
                    if (0 == undeterminedNbCount[prevMousePos][catPos][prevTurn]) {
                        // All "neighbours of StateTuple" are determined to be winning of the opponent.
                        dp[prevMousePos][catPos][prevTurn] = CAT_WINS;
                        if (debug) {
                            printf("%*s#2,dp[mousePos:%d][catPos:%d][turn:%d] is just determined to be CAT_WINS by depletion\n", indent, "", prevMousePos, catPos, prevTurn);
                        }
                    }                    
                }
                
                if (INVALID == dp[prevMousePos][catPos][prevTurn]) {
                    // Only propagates from determined "StateTuple"s.
                    continue;
                }
                
                bottomUpDfs(prevMousePos, catPos, prevTurn, graph, level+1);
            }
        } else {
            int prevTurn = !mouseOrCatTurn; // CAT_TURN
            for (auto prevCatPos : graph[catPos]) {
                if (openSet[mousePos][prevCatPos][prevTurn]) continue;
                if (INVALID != dp[mousePos][prevCatPos][prevTurn]) {
                    // Already propagated when it was just determined.
                    continue;
                }
                
                if (dp[mousePos][catPos][mouseOrCatTurn] == CAT_WINS) {
                    undeterminedNbCount[mousePos][prevCatPos][prevTurn] = 0;
                    dp[mousePos][prevCatPos][prevTurn] = CAT_WINS;
                    if (debug) {
                        printf("%*s#1,dp[mousePos:%d][catPos:%d][turn:%d] is just determined to be CAT_WINS, by dp[nextMousePos:%d][nextCatPos:%d][nextTurn:%d] being CAT_WINS\n", indent, "", mousePos, prevCatPos, prevTurn, mousePos, catPos, mouseOrCatTurn);
                    }
                } else {
                    --undeterminedNbCount[mousePos][prevCatPos][prevTurn];
                    if (debug) {
                        printf("%*sundeterminedNbCount[mousePos:%d][catPos:%d][turn:%d] is decremented to %d by dp[nextMousePos:%d][nextCatPos:%d][nextTurn:%d] being MOUSE_WINS\n", indent, "", mousePos, prevCatPos, prevTurn, undeterminedNbCount[mousePos][prevCatPos][prevTurn], mousePos, catPos, mouseOrCatTurn);
                    }
                    if (0 == undeterminedNbCount[mousePos][prevCatPos][prevTurn]) {
                        // All "neighbours of StateTuple" are determined to be winning of the opponent.
                        dp[mousePos][prevCatPos][prevTurn] = MOUSE_WINS;
                        if (debug) {
                            printf("%*s#2,dp[mousePos:%d][catPos:%d][turn:%d] is just determined to be MOUSE_WINS by depletion\n", indent, "", mousePos, prevCatPos, prevTurn);
                        }
                    }    
                }
                
                if (INVALID == dp[mousePos][prevCatPos][prevTurn]) {
                    // Only propagates from determined "StateTuple"s.
                    continue;
                }
                
                bottomUpDfs(mousePos, prevCatPos, prevTurn, graph, level+1);
            }        
        }
        
        openSet[mousePos][catPos][mouseOrCatTurn] = false;     
        closedSet[mousePos][catPos][mouseOrCatTurn] = true;     
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
                if (0 == catPos) {
                    // However the cat is in the hole, it's deemed "lost".
                    undeterminedNbCount[mousePos][catPos][MOUSE_TURN] = 0;
                    undeterminedNbCount[mousePos][catPos][CAT_TURN] = 0;
                    dp[mousePos][catPos][MOUSE_TURN] = MOUSE_WINS;
                    dp[mousePos][catPos][CAT_TURN] = MOUSE_WINS;
                } else if (0 == mousePos) {
                    undeterminedNbCount[mousePos][catPos][MOUSE_TURN] = 0;
                    undeterminedNbCount[mousePos][catPos][CAT_TURN] = 0;
                    dp[mousePos][catPos][MOUSE_TURN] = MOUSE_WINS;
                    dp[mousePos][catPos][CAT_TURN] = MOUSE_WINS;
                } else if (catPos == mousePos) {
                    undeterminedNbCount[mousePos][catPos][MOUSE_TURN] = 0;
                    undeterminedNbCount[mousePos][catPos][CAT_TURN] = 0;
                    dp[mousePos][catPos][MOUSE_TURN] = CAT_WINS;
                    dp[mousePos][catPos][CAT_TURN] = CAT_WINS;
                } else {
                    undeterminedNbCount[mousePos][catPos][MOUSE_TURN] = graph[mousePos].size();
                    undeterminedNbCount[mousePos][catPos][CAT_TURN] = graph[catPos].size();
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
        - the dfs stack then pops back to "u1" and goes on to "v"again, now that both "u3" and "u4" are "not-in-openset/not-on-stack StateTupleNeighbours" of "v", shall we check them? yes but again they'll immediately return to "v", remaining undetermined too;
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
    
        return INVALID == dp[1][2][MOUSE_TURN] ? DRAW : dp[1][2][MOUSE_TURN];
    }
};
