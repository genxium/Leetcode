class Solution {
public:
    string getHint(string secret, string guess) {
      /*
      It's guaranteed by the problem description that "secret.length() == guess.length()".
      */
      int len = secret.length();
      int bullsCount = 0;
      unordered_multiset<char> cowsCandidatesFromSecret;
      unordered_multiset<char> cowsCandidatesFromGuess;

      for (int i = 0; i < len; ++i) {
        if (secret[i] == guess[i]) {
          ++bullsCount;
        } else {
          cowsCandidatesFromSecret.insert(secret[i]);
          cowsCandidatesFromGuess.insert(guess[i]);
        }
      }
      
      int cowsCount = 0;
      for (auto &guessCh : cowsCandidatesFromGuess) {
        auto it = cowsCandidatesFromSecret.find(guessCh);
        if (it != cowsCandidatesFromSecret.end()) {
          ++cowsCount;
          cowsCandidatesFromSecret.erase(it);
        }
      }
      
      string ans = to_string(bullsCount) + "A" + to_string(cowsCount) + "B";
      return ans;
    }
};
