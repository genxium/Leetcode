/*
This solution is NOT authored by me. I've just renamed some variables and commented.
*/

int const BRANCHING_FACTOR = 26;
struct TrieNode {
  TrieNode* children[BRANCHING_FACTOR];
  vector<int> prefXList; // a value "x" in "TrieNode->prefXList" means that "words[x]" has travelled thru the current "TrieNode" 
  vector<int> revPrefXList; // a value "x" in "TrieNode->revPrefXList" means that "words[x].reverse()" has travelled thru the current "TrieNode"

  TrieNode() {
    for (int i = 0; i < BRANCHING_FACTOR; ++i) {
      children[i] = nullptr;
    }
    prefXList.clear();
    revPrefXList.clear();
  }
};

struct Trie {
  TrieNode* root;

  Trie() {
    root = new TrieNode();
  }

  void insert(string& word, int x) {
    TrieNode* trav = root;
    for (int i = 0; i < word.length(); ++i) {
      int branch = word[i]-'a';
      if (!trav->children[branch]) {
        trav->children[branch] = new TrieNode();
      }
      trav = trav->children[branch];
      trav->prefXList.push_back(x);
    }
    trav = root;
    for (int i = word.length() - 1; i >= 0; --i) {
      int branch = word[i]-'a';
      if (!trav->children[branch]) {
        trav->children[branch] = new TrieNode();
      }
      trav = trav->children[branch];
      trav->revPrefXList.push_back(x);
    }
  }

  int check(string& prefStr, string& revPrefStr) {
    TrieNode* trav = root;
    TrieNode* prefNode = NULL, *revPrefNode = NULL;
    assert(prefStr.length() > 0 || revPrefStr.length() > 0);
    for (int i = 0; i < prefStr.length(); ++i) {
      int branch = prefStr[i]-'a';
      if (!trav->children[branch]) {
        return -1;
      }
      trav = trav->children[branch];
    }
    if (!trav) {
      return -1;
    } else {
      prefNode = trav;
    }

    trav = root;
    for (int i = 0; i < revPrefStr.length(); ++i) {
      int branch = revPrefStr[i]-'a';
      if (!trav->children[branch]) {
        return -1;
      }
      trav = trav->children[branch];
    }
    if (!trav) {
      return -1;
    } else {
      revPrefNode = trav;
    }
    vector<int>& prefXList = prefNode->prefXList;
    vector<int>& revPrefXList = revPrefNode->revPrefXList;
    if (revPrefStr.length() == 0) {
      return prefXList.back();
    }
    if (prefStr.length() == 0) {
      return revPrefXList.back();
    }
    int prefIdx = prefXList.size() - 1;
    int revPrefIdx = revPrefXList.size() - 1;
    while (prefIdx >= 0 && revPrefIdx >= 0) {
      if (prefXList[prefIdx] == revPrefXList[revPrefIdx]) {
        return prefXList[prefIdx];
      }
      if (prefXList[prefIdx] < revPrefXList[revPrefIdx]) {
        --revPrefIdx;
      } else {
        --prefIdx;
      }
    }
    return -1;
  }
};


class WordFilter {
  private:
    int wordsCount;
    Trie trie;
  public:
    WordFilter(vector<string>& words) {
      wordsCount = words.size();
      for (int i = 0; i < wordsCount; i++) {
        trie.insert(words[i], i);
      }
    }

    int f(string prefix, string suffix) {
      if (prefix.empty() && suffix.empty()) {
        return wordsCount - 1;
      }
      reverse(suffix.begin(), suffix.end());  
      // now that "suffix" becomes a "reverse-prefix"
      return trie.check(prefix, suffix);
    }
};

/**
 * Your WordFilter object will be instantiated and called as such:
 * WordFilter* obj = new WordFilter(words);
 * int param_1 = obj->f(prefix,suffix);
 */

