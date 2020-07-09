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

void startsWithInTrie(TrieNode *root, int &idx, string &prefix, bool &found) {
  if (true == found) {
    return;
  }
  if (idx == prefix.length()) {
    found = true;
    return;
  }
  char c = prefix[idx];
  if (root->branches.find(c) != root->branches.end()) {
    startsWithInTrie(root->branches[c], ++idx, prefix, found);
  }
}

void searchInTrie(TrieNode *root, int &idx, string &word, bool &found) {
  if (true == found) {
    return;
  }
  if (idx == word.length()) {
    found = (0 == root->word.compare(word));
    return;
  }
  char c = word[idx];
  if (root->branches.find(c) != root->branches.end()) {
    searchInTrie(root->branches[c], ++idx, word, found);
  }
}

class Trie {
private:
    TrieNode *root;
public:
    /** Initialize your data structure here. */
    Trie() {
      root = new TrieNode('/');
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
      int idx = 0;
      insertToTrie(this->root, idx, word);
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
      bool found = false;
      int idx = 0;
      searchInTrie(this->root, idx, word, found);
      return found;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
      bool found = false;
      int idx = 0;
      startsWithInTrie(this->root, idx, prefix, found);
      return found;
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */
