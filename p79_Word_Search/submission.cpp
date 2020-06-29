#define MAXN 200
#define MAXL 1000

vector<int> diry{+1, -1, 0, 0}; // +1: down
vector<int> dirx{0, 0, -1, +1}; // +1: right
bool visited[MAXN][MAXN]; // Can I use a "map<string, map<int, bool>> visited" for "visited[prefixStr][y*xUpper+x]" instead to avoid backtracking, and thus optimize time-complexity? 

class Solution {
public:
    void search(vector<vector<char>>& board, int y, int x, int &yUpper, int &xUpper, int offsetInWord, string &word, bool visited[][MAXN], bool &ans) {
        if (ans) {
            return;
        }
        
        if (board[y][x] != word[offsetInWord]) {
            return;
        }
        
        if (offsetInWord+1 == word.size()) {
            //printf("found match at board[y:%d][x:%d] == %c\n", y, x, board[y][x]);
            ans = true;
            return;
        }
        
        visited[y][x] = true;
        for (int i = 0; i < 4; ++i) {
            int nby = y+diry[i];
            int nbx = x+dirx[i];
            if (0 > nby || nby >= yUpper) continue;
            if (0 > nbx || nbx >= xUpper) continue;
            if (visited[nby][nbx]) continue;
            search(board, nby, nbx, yUpper, xUpper, offsetInWord+1, word, visited, ans);
            if (ans) return;
        }
        visited[y][x] = false; // backtracking
    }
    
    bool exist(vector<vector<char>>& board, string word) {
        /*
        test case#1
        [["a"]]
        "a"
        
        test case#2
        [["a", "a"]]
        "aaa"
        */
        bool ans = false;
        int yUpper = board.size();
        if (0 == yUpper) return false;
        int xUpper = board[0].size();
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                memset(visited, false, sizeof(visited));
                search(board, y, x, yUpper, xUpper, 0, word, visited, ans);
                if (ans) return true;
            }
        }
        return ans;
    }
};
