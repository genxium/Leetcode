bool const debug = false;
bool const debug_undetermined_nb_count = false;
int const MAXN = 64;
int const MAXTURNS = 1000;
int const MOUSE_WINS = 1, CAT_WINS = 2, INVALID = -1;   
int const MOUSE_TURN = 0, CAT_TURN = 1;
typedef vector<int> VI;
typedef unordered_set<int> USI;

VI const dy = {0, 0, -1, +1};
VI const dx = {-1, +1, 0, 0};
/*
A "StateTuple" is {mousePos, catPos, mouseOrCatTurn}.
*/
int mouseInitPos, catInitPos;
int dp[MAXN][MAXN][MAXTURNS];
int undeterminedNbCount[MAXN][MAXN][MAXTURNS]; // [WARNING] Refers to the "neighbour of StateTupleGraph" instead of the "OriginalGraph".
bool openSet[MAXN][MAXN][MAXTURNS]; // a.k.a. "onStack[MAXN][MAXN][2]"
bool closedSet[MAXN][MAXN][MAXTURNS]; // a.k.a. "wasOnButNowOffStack[MAXN][MAXN][2]"

/*
test cases
["####F","#C...","M...."]
1
2
["M.C...F"]
1
4
["M.C...F"]
1
3
["C...#","...#F","....#","M...."]
2
5
[".M...","..#..","#..#.","C#.#.","...#F"]
3
1
["#..CF",".M.#.","...#.","#....",".....","....#"]
4
3
*/
class Solution {
public:
    void bottomUpDfs(int mousePos, int catPos, int turnNum, vector<USI> &mouseGraph, vector<USI> &catGraph, int level) {
        if (closedSet[mousePos][catPos][turnNum]) {
            return;
        }
        
        if (INVALID != dp[mouseInitPos][catInitPos][0]) {
            return;   
        }
        
        do {
            int mouseOrCatTurn = (turnNum%2);
            int indent = 2*level;
            openSet[mousePos][catPos][turnNum] = true;
            
            if (turnNum == 0) break;
            if (mouseOrCatTurn == CAT_TURN) {
                int prevTurn = turnNum-1; // MOUSE_TURN
                for (auto prevMousePos : mouseGraph[mousePos]) {
                    if (openSet[prevMousePos][catPos][prevTurn] || closedSet[prevMousePos][catPos][prevTurn]) continue;
                    if (INVALID != dp[prevMousePos][catPos][prevTurn]) {
                        // Already propagated when it was just determined.
                        continue;
                    }
                    if (dp[mousePos][catPos][turnNum] == MOUSE_WINS) {
                        undeterminedNbCount[prevMousePos][catPos][prevTurn] = 0;
                        dp[prevMousePos][catPos][prevTurn] = MOUSE_WINS;
                        if (debug) {
                            printf("%*s#1,dp[mousePos:%d][catPos:%d][turn:%d] is just determined to be MOUSE_WINS, by dp[nextMousePos:%d][nextCatPos:%d][nextTurn:%d] being MOUSE_WINS\n", indent, "", prevMousePos, catPos, prevTurn, mousePos, catPos, turnNum);
                        }
                    } else {
                        --undeterminedNbCount[prevMousePos][catPos][prevTurn];
                        if (debug && debug_undetermined_nb_count) {
                            printf("%*sundeterminedNbCount[mousePos:%d][catPos:%d][turn:%d] is decremented to %d by dp[nextMousePos:%d][nextCatPos:%d][nextTurn:%d] being CAT_WINS\n", indent, "", prevMousePos, catPos, prevTurn, undeterminedNbCount[prevMousePos][catPos][prevTurn], mousePos, catPos, turnNum);
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

                    bottomUpDfs(prevMousePos, catPos, prevTurn, mouseGraph, catGraph, level+1);
                }
            } else {
                int prevTurn = turnNum-1; // MOUSE_TURN
                for (auto prevCatPos : catGraph[catPos]) {
                    if (openSet[mousePos][prevCatPos][prevTurn]) continue;
                    if (INVALID != dp[mousePos][prevCatPos][prevTurn]) {
                        // Already propagated when it was just determined.
                        continue;
                    }

                    if (dp[mousePos][catPos][turnNum] == CAT_WINS) {
                        undeterminedNbCount[mousePos][prevCatPos][prevTurn] = 0;
                        dp[mousePos][prevCatPos][prevTurn] = CAT_WINS;
                        if (debug) {
                            printf("%*s#1,dp[mousePos:%d][catPos:%d][turn:%d] is just determined to be CAT_WINS, by dp[nextMousePos:%d][nextCatPos:%d][nextTurn:%d] being CAT_WINS\n", indent, "", mousePos, prevCatPos, prevTurn, mousePos, catPos, turnNum);
                        }
                    } else {
                        --undeterminedNbCount[mousePos][prevCatPos][prevTurn];
                        if (debug & debug_undetermined_nb_count) {
                            printf("%*sundeterminedNbCount[mousePos:%d][catPos:%d][turn:%d] is decremented to %d by dp[nextMousePos:%d][nextCatPos:%d][nextTurn:%d] being MOUSE_WINS\n", indent, "", mousePos, prevCatPos, prevTurn, undeterminedNbCount[mousePos][prevCatPos][prevTurn], mousePos, catPos, turnNum);
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

                    bottomUpDfs(mousePos, prevCatPos, prevTurn, mouseGraph, catGraph, level+1);
                }        
            }
        } while (false);
        
        openSet[mousePos][catPos][turnNum] = false;     
        closedSet[mousePos][catPos][turnNum] = true;     
    }

    bool canMouseWin(vector<string>& grid, int catJump, int mouseJump) {
        // init
        memset(dp, INVALID, sizeof dp);
        memset(undeterminedNbCount, INVALID, sizeof undeterminedNbCount);
        memset(openSet, false, sizeof openSet);
        memset(closedSet, false, sizeof closedSet);
        mouseInitPos = INVALID, catInitPos = INVALID;
        int foodPos = INVALID;
        
        int yUpper = grid.size(), xUpper = grid[0].size();
        int n = yUpper*xUpper;
        vector<USI> mouseGraph, catGraph;   
        
        for (int pos = 0; pos < n; ++pos) {
            int y = pos/xUpper, x = pos%xUpper;
            if (grid[y][x] == 'M') mouseInitPos = pos;
            if (grid[y][x] == 'C') catInitPos = pos;
            if (grid[y][x] == 'F') foodPos = pos;
            mouseGraph.push_back({});
            catGraph.push_back({});
            if (grid[y][x] == '#') continue;
            int catDirBlocked[4] = {false, false, false, false};
            for (int i = 0; i <= catJump; ++i) {
                for (int dir = 0; dir < 4; ++dir) {
                    if (catDirBlocked[dir]) continue;
                    int newPosY = y+i*dy[dir], newPosX = x+i*dx[dir];
                    if (newPosY >= yUpper || newPosY < 0 || newPosX >= xUpper || newPosX < 0) continue;
                    if ('#' == grid[newPosY][newPosX]) {
                        catDirBlocked[dir] = true;
                        continue;
                    }
                    int newPos = newPosY*xUpper+newPosX; 
                    catGraph[pos].insert(newPos);
                }
            }
            
            int mouseDirBlocked[4] = {false, false, false, false};
            for (int i = 0; i <= mouseJump; ++i) {
                for (int dir = 0; dir < 4; ++dir) {
                    if (mouseDirBlocked[dir]) continue;
                    int newPosY = y+i*dy[dir], newPosX = x+i*dx[dir];
                    if (newPosY >= yUpper || newPosY < 0 || newPosX >= xUpper || newPosX < 0) continue;
                    if ('#' == grid[newPosY][newPosX]) {
                        mouseDirBlocked[dir] = true;
                        continue;
                    }
                    int newPos = newPosY*xUpper+newPosX; 
                    mouseGraph[pos].insert(newPos);
                }
            }
        }
        
        for (int mousePos = 0; mousePos < n; ++mousePos) {
            for (int catPos = 0; catPos < n; ++catPos) {
                for (int turnNum = 0; turnNum < 1000; ++turnNum) {
                    if (mousePos == catPos) {
                        if (mousePos != foodPos) {
                            undeterminedNbCount[mousePos][catPos][turnNum] = 0;
                            dp[mousePos][catPos][turnNum] = CAT_WINS;
                            continue;
                        }
                    } else {
                        // mousePos != catPos
                        if (mousePos == foodPos) {
                            undeterminedNbCount[mousePos][catPos][turnNum] = 0;
                            dp[mousePos][catPos][turnNum] = MOUSE_WINS;
                            continue;
                        }
                        if (catPos == foodPos) {
                            undeterminedNbCount[mousePos][catPos][turnNum] = 0;
                            dp[mousePos][catPos][turnNum] = CAT_WINS;
                            continue;
                        }                     
                    }
                    
                    int mouseOrCatTurn = (turnNum%2);
                    if (mouseOrCatTurn == MOUSE_TURN) {
                        undeterminedNbCount[mousePos][catPos][turnNum] = mouseGraph[mousePos].size();
                    } else {
                        undeterminedNbCount[mousePos][catPos][turnNum] = catGraph[catPos].size();
                    }
                }
            }
        }
        
        // loop
        for (int turnNum = 0; turnNum < 1000 && INVALID == dp[mouseInitPos][catInitPos][0]; ++turnNum) {
            for (int mousePos = 0; mousePos < n && INVALID == dp[mouseInitPos][catInitPos][0]; ++mousePos) {
                for (int catPos = 0; catPos < n && INVALID == dp[mouseInitPos][catInitPos][0]; ++catPos) {
                    if (INVALID == dp[mousePos][catPos][turnNum]) continue;
                    bottomUpDfs(mousePos, catPos, turnNum, mouseGraph, catGraph, 0);
                }
            }
        }
    
        int result = INVALID == dp[mouseInitPos][catInitPos][0] ? CAT_WINS : dp[mouseInitPos][catInitPos][0]; 
        return result == MOUSE_WINS;

    }
};
