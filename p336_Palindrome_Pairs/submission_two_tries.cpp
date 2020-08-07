int const INVALID = -1;
int const BRANCHING_FACTOR = 27;
int const END_BRANCH = 26;

struct TrieNode {
  TrieNode* children[BRANCHING_FACTOR];
  int endIdx;
  TrieNode() {
    for (int i = 0; i < BRANCHING_FACTOR; ++i) {
      children[i] = nullptr;
    }
    endIdx = INVALID;
  }
};

class Solution {
public:
    int pairHash(int first, int second, int n) {
        return first*n + second;
    }
    
    pair<int, int> unpairHash(int val, int n) {
        int second = val%n;
        int first = val/n;
        return {first, second};
    }
    
    bool matchesExpectedLength(int x, vector<int> palindromeLenCountAtPos) {
        for (int j = 0; j < palindromeLenCountAtPos.size(); ++j) {
            if (j == 0 && palindromeLenCountAtPos[j] > 1) {
                if (palindromeLenCountAtPos[j] >= x) return true;
            }
            if (palindromeLenCountAtPos[j] == x) return true;
        }
        return false;
    }
    
    void longestPalindrome(string &s, vector< vector<int> > &palindromeLenCount) {
      int slen = s.length();
      if (slen == 0) return;
        
      // init
      palindromeLenCount.push_back({1}); // A single character itself is of course a palindrome.

      if (slen <= 1) return;

      // loop
      for (int i = 1; i < slen; ++i) {
        vector<int> currentPalindromeLenCount = {1}; // A single character itself is of course a palindrome.
        vector<int> prevPalindromeLenCount = palindromeLenCount[i - 1];
        /*
           Re-purpose "currentPalindromeLenCount[0]" to contain the length of the "repeating character palindrome ended at each s[i]", which must be a palindrome by itself.

           Without such "re-purposing", besides the palindromes of pattern "aaaa....aaaa"(all the same), the following iteration of "j" is only of O(log2(i)) time-complexity. 

           Therefore I try to tackle the edge-case of "repeating character palindrome ended at each s[i]".
         */
        for (int j = 0; j < prevPalindromeLenCount.size(); ++j) {
          int prevPalindromeLen = prevPalindromeLenCount[j]; // could be ODD or EVEN
          int leftIdxCandidateOfCurrentPalindrome = -1, newPalindromeLen = 0;
          do {
            if (0 < (prevPalindromeLen & 1)) {
              // ODD
              if (0 == j) {
                if (1 < prevPalindromeLen) {
                  // A "repeating character palindrome", otherwise "prevPalindromeLenCount[0]" should be 1.
                  if (s[i] == s[i-1]) {
                    currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                    leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                    newPalindromeLen = currentPalindromeLenCount[0];
                    break;             
                  }
                } else {
                  // 1 == prevPalindromeLen
                  if (s[i] == s[i-1]) {
                    currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                    leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                    newPalindromeLen = currentPalindromeLenCount[0];
                    break;                  
                  }
                }
              }
            } else {
              // EVEN
              if (0 == j) {
                // A "repeating character palindrome", otherwise "prevPalindromeLenCount[0]" should be 1 which is ODD.
                if (s[i] == s[i-1]) {
                  /* 
                     Joins the "repeating character palindrome", it's IMPOSSIBLE that 
                     ```
                     s[(i-1)-(prevPalindromeLen - 1)-1] == s[i]
                     ```
                     , otherwise "prevPalindromeLen" should be ODD instead and bigger.
                   */
                  currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                  leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                  newPalindromeLen = currentPalindromeLenCount[0];
                  break;
                }
              }
            }

            int leftIdxOfPrevPalindrome = (i - 1) - (prevPalindromeLen - 1); 
            leftIdxCandidateOfCurrentPalindrome = (leftIdxOfPrevPalindrome - 1); 
            if (0 > leftIdxCandidateOfCurrentPalindrome) break;
            char leftCandidate = s[leftIdxCandidateOfCurrentPalindrome];
            if (leftCandidate != s[i]) break;
            newPalindromeLen = (prevPalindromeLen + 2);
            currentPalindromeLenCount.push_back(newPalindromeLen); 
          } while (false);

          if (0 == newPalindromeLen) {
            continue;
          }
        }  
        palindromeLenCount.push_back(currentPalindromeLenCount);
      }
    }
    
    vector<vector<int>> palindromePairs(vector<string>& words) {
        /*
        test case #1
        ["a","b","c","ab","ac","aa"]
        
        test case #2
        ["abcd","dcba","lls","s","sssll", ""]
        
        test case #3
        ["bb","bababab","baab","abaabaa","aaba","","bbaa","aba","baa","b"]
        */
        TrieNode *root = new TrieNode();
        TrieNode *revroot = new TrieNode();

        unordered_set<int> ansSet;
        vector<vector<int>> ans;
        
        int n = words.size();
        for (int k = 0; k < n; ++k) {
            string word = words[k];
            int l = word.length(); // Is it possible that "l == 0" for some words[k] ?
            
            /*
            insert into revroot
            */ 
            TrieNode *trav = revroot;
            for (int i = l-1; i >= 0; --i) {
                int branch = word[i] - 'a';
                if (!trav->children[branch]) {
                    trav->children[branch] = new TrieNode();
                }
                trav = trav->children[branch];
            }
            // wrap up each word with END_BRANCH
            trav->children[END_BRANCH] = new TrieNode();
            trav->children[END_BRANCH]->endIdx = k;
            
            /*
            insert into root
            */
            trav = root;
            for (int i = 0; i < l; ++i) {
                int branch = word[i] - 'a';
                if (!trav->children[branch]) {
                    trav->children[branch] = new TrieNode();
                }
                trav = trav->children[branch];
            }
            // wrap up each word with END_BRANCH
            trav->children[END_BRANCH] = new TrieNode();
            trav->children[END_BRANCH]->endIdx = k;
        }
        
        for (int k = 0; k < n; ++k) {
            string word = words[k];
            vector< vector<int> > palindromeLenCount; // "palindromeLenCount[i][j] == x" means that "the j-th palindrome ended at word[i] has length x" 
            longestPalindrome(word, palindromeLenCount);
            
            int l = word.length();
            //printf("checking words[k:%d] %s\n", k, word.c_str());
            
            /*
            check it in "revroot"
            */
            //printf("\tcheck it in revroot\n");
            TrieNode *trav = revroot;
            for (int i = 0; i < l; ++i) {
                int branch = word[i] - 'a';
                bool canGoDown = true;
                if (trav->children[branch]) {
                    trav = trav->children[branch];  
                    //printf("\t@i:%d, word[i]:%c running down\n", i, word[i]);
                } else {
                    canGoDown = false;
                } 
                
                if (!canGoDown) {
                    // no more prefix of "word" exists 
                    break;
                }
                
                if (trav->children[END_BRANCH]) {
                    int kk = trav->children[END_BRANCH]->endIdx;
                    if (kk == k) continue;
                    //printf("\t@i:%d, got a prefix end to check, kk:%d\n", i, kk);
                    int ll = words[kk].length();
                    if (ll > l) continue;
                    
                    if (i == l-1) {
                        //printf("\t\tadding {k:%d, kk:%d}#1\n", k, kk);
                        ansSet.insert(pairHash(k, kk, n));
                    } else {
                        // words[k][0, ..., i] existed as reverse(words[kk]), if the rest "l-(i+1)" characters of words[k] form a palindrome, then "words[k].concat(words[kk])" is palindromic
                        //printf("\t\tpalindromeLenCount[l-1:%d].size() == %d\n", l-1, palindromeLenCount[l-1].size());
                        if (matchesExpectedLength(l-(i+1), palindromeLenCount[l-1])) {
                            //printf("\t\tadding {k:%d, kk:%d}#2\n", l-(i+1), k, kk);
                            ansSet.insert(pairHash(k, kk, n));
                        }
                    }
                }
            }
            // wrap up, consider only `words[kk] == ""`
            if (l > 0 && revroot->children[END_BRANCH]) {
                int kk = revroot->children[END_BRANCH]->endIdx;
                if (matchesExpectedLength(l, palindromeLenCount[l-1])) {
                    //printf("\t\tadding {k:%d, kk:%d}#3\n", k, kk);
                    ansSet.insert(pairHash(k, kk, n));
                }
            }       
            
            /*
            check it in "root"
            */
            //printf("\tcheck it in root\n");
            trav = root;
            for (int i = l-1; i >= 0; --i) {
                int branch = word[i] - 'a';
                bool canGoDown = true;
                if (trav->children[branch]) {
                    trav = trav->children[branch];  
                    //printf("\t@i:%d, word[i]:%c running down\n", i, word[i]);
                } else {
                    canGoDown = false;
                } 
                
                if (!canGoDown) {
                    // no more reverse-suffix of "word" exists 
                    break;
                }
                
                if (trav->children[END_BRANCH]) {
                    int kk = trav->children[END_BRANCH]->endIdx;
                    if (kk == k) continue;
                    //printf("\t@i:%d, got a prefix end to check, kk:%d\n", i, kk);
                    int ll = words[kk].length();
                    if (ll > l) continue;
                    
                    if (i == 0) {
                        //printf("\t\tadding {kk:%d, k:%d}#1\n", kk, k);
                        ansSet.insert(pairHash(kk, k, n));
                    } else {
                        // reverse(words[k][i, ..., l-1]) existed as (words[kk]), if the rest "i" characters of words[k] form a palindrome, then "words[kk].concat(words[k])" is palindromic
                        //printf("\t\tpalindromeLenCount[i-1:%d].size() == %d\n", i-1, palindromeLenCount[i-1].size());
                        if (matchesExpectedLength(i, palindromeLenCount[i-1])) {
                            //printf("\t\tadding {kk:%d, k:%d}#2\n", kk, k);
                            ansSet.insert(pairHash(kk, k, n));
                        }
                    }
                }
            }
            
            // wrap up, consider only `words[kk] == ""`
            if (l > 0 && root->children[END_BRANCH]) {
                int kk = root->children[END_BRANCH]->endIdx;
                if (matchesExpectedLength(l, palindromeLenCount[l-1])) {
                    //printf("\t\tadding {kk:%d, k:%d}#3\n", kk, k);
                    ansSet.insert(pairHash(kk, k, n));
                }
            }
        }
        for (auto &val : ansSet) {
            pair<int, int> thePair = unpairHash(val, n);
            ans.push_back({thePair.first, thePair.second});
        }
        return ans;
    }
};
