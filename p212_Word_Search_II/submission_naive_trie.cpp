#define MAXN 1000
bool visited[MAXN*MAXN];
vector<int> diry{+1, -1, 0, 0}; // +1 is down
vector<int> dirx{0, 0, +1, -1}; // +1 is right

class TrieNode {
public:
  char c;
  string word;
  unordered_map<char, TrieNode*> branches;
  TrieNode(char aC) {
    this->c = aC;
    this->word = "";
  }
};

void insertToTrie(TrieNode *root, int &idx, string &word) {
  if (idx == word.length()) {
    root->word = word.substr(0, word.length());
    return;
  }
  char c = word[idx];
  if (root->branches.find(c) == root->branches.end()) {
    root->branches[c] = new TrieNode(c);
  }
  insertToTrie(root->branches[c], ++idx, word);
}

void searchInTrie(TrieNode *root, int y, int x, int &yUpper, int &xUpper, vector<vector<char>>& board, set<string> &ans) {
  char c = board[y][x];
  int idx = y*xUpper + x;
  visited[idx] = true;
  if (root->branches.find(c) != root->branches.end()) {
    TrieNode *branch = root->branches[c]; 
    if (false == branch->word.empty()) {
      ans.insert(branch->word);
    }
    for (int i = 0; i < 4; ++i) {
      int ny = y+diry[i], nx = x+dirx[i];
      if (0 > ny || 0 > nx || yUpper <= ny || xUpper <= nx) {
        continue;
      }
      int nidx = ny*xUpper + nx;
      if (visited[nidx])  continue;
      searchInTrie(branch, ny, nx, yUpper, xUpper, board, ans);
    } 
  }
  visited[idx] = false; // backtracking
}

class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
      int yUpper = board.size();
      if (0 >= yUpper) return {};
      int xUpper = board[0].size();
      TrieNode *root = new TrieNode('/'); // this solution doesn't take responsibility of deallocation
      for (auto &word : words) {
        int idx = 0;
        insertToTrie(root, idx, word);
      }
      
      memset(visited, false, sizeof(visited));
      set<string> ans; // using a set for deduplication
      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          searchInTrie(root, y, x, yUpper, xUpper, board, ans);
        }
      }
      
      vector<string> toRet;
      for (auto &s : ans) {
        toRet.push_back(s);
      }
      
      return toRet;
    }
};
