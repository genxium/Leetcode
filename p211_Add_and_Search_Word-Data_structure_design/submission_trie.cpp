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

void searchInTrie(TrieNode *root, int idx, string &word, bool &found) {
  if (true == found) {
    return;
  }
  if (idx == word.length()) {
    found = (false == root->word.empty());
    return;
  }
  if ('.' == word[idx]) {
    for (auto &it : root->branches) {
      searchInTrie(it.second, idx+1, word, found);
    }
  } else {
    char c = word[idx];
    if (root->branches.find(c) != root->branches.end()) {
      searchInTrie(root->branches[c], ++idx, word, found);
    }  
  }
}

class WordDictionary {
private:
    TrieNode *root;
public:
    /** Initialize your data structure here. */
    WordDictionary() {
      root = new TrieNode('/');
    }
    
    /** Adds a word into the data structure. */
    void addWord(string word) {
      int idx = 0;
      insertToTrie(this->root, idx, word);
    }
    
    /** Returns if the word is in the data structure. A word could contain the dot character '.' to represent any one letter. */
    bool search(string word) {
      bool found = false;
      int idx = 0;
      searchInTrie(this->root, idx, word, found);
      return found;
    }
};

/**
 * Your WordDictionary object will be instantiated and called as such:
 * WordDictionary* obj = new WordDictionary();
 * obj->addWord(word);
 * bool param_2 = obj->search(word);
 */
