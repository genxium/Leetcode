class Solution {
public:
    string getHint(string secret, string guess) {
      /*
      It's guaranteed by the problem description that "secret.length() == guess.length()".
      */
      int len = secret.length();
      int bullsCount = 0, cowsCount = 0;
      unordered_multiset<char> cowsCandidatesFromSecret;
      unordered_multiset<char> cowsCandidatesFromGuess;

      for (int i = 0; i < len; ++i) {
        if (secret[i] == guess[i]) {
          ++bullsCount;
        } else {
          auto it1 = cowsCandidatesFromSecret.find(guess[i]);
          if (it1 != cowsCandidatesFromSecret.end()) {
            ++cowsCount;
            cowsCandidatesFromSecret.erase(it1);
          } else {
            cowsCandidatesFromGuess.insert(guess[i]); // for later matching
          }
          
          auto it2 = cowsCandidatesFromGuess.find(secret[i]);
          if (it2 != cowsCandidatesFromGuess.end()) {
            ++cowsCount;
            cowsCandidatesFromGuess.erase(it2);
          } else {
            cowsCandidatesFromSecret.insert(secret[i]); // for later matching
          }
        }
      }
      
      string ans = to_string(bullsCount) + "A" + to_string(cowsCount) + "B";
      return ans;
    }
};
