int const INVALID = -1;
int const BRANCHING_FACTOR = 27;
int const END_BRANCH = 26;

class TrieNode {
public:
    TrieNode* children[BRANCHING_FACTOR];
    TrieNode() {
        for (int i = 0; i < BRANCHING_FACTOR; ++i) {
            children[i] = nullptr;
        }
    }

    void insert(string &word) {
        auto trav = this;
        for (int i = 0; i < word.length(); ++i) {
            int branch = word[i] - 'a';
            if (!trav->children[branch]) {
                trav->children[branch] = new TrieNode();
            }
            trav = trav->children[branch];
        }
        // wrap up each word with END_BRANCH
        trav->children[END_BRANCH] = new TrieNode();
    }
    
    string earliestEndingWord(string &target) {
        auto trav = this;
        string toRet = "";
        for (int i = 0; i < target.length(); ++i) {
            int branch = target[i] - 'a';
            if (trav->children[END_BRANCH]) {
                break;
            }
            toRet.push_back(target[i]);
            if (!trav->children[branch]) {
                return target; // no match
            }
            trav = trav->children[branch];
        }
        return toRet;
    }
};

/*
test cases
["cat","bat","rat"]
"the cattle was rattled by the battery"
["a","b","c"]
"aadsfasf absbs bbab cadsfafs"
["a", "aa", "aaa", "aaaa"]
"a aa a aaaa aaa aaa aaa aaaaaa bbb baba ababa"
["catt","cat","bat","rat"]
"the cattle was rattled by the battery"
["ac","ab"]
"it is abnormal that this solution is accepted"
*/
class Solution {
public:
    string replaceWords(vector<string>& dictionary, string sentence) {
        string buff = "";
        string ans = "";
        TrieNode* root = new TrieNode();
        for (auto &word : dictionary) root->insert(word);
        for (auto ch : sentence) {
            if (' ' == ch) {
                string tmp = root->earliestEndingWord(buff);
                ans += (ans.empty() ? tmp : " " + tmp);
                buff = "";
            } else {
                buff.push_back(ch);
            }
        }
        
        // wrap up
        string tmp = root->earliestEndingWord(buff);
        ans += (ans.empty() ? tmp : " " + tmp);        
        
        return ans;
    }
};
