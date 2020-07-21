class Solution {
public:
    string getHint(string secret, string guess) {
      /*
      It's guaranteed by the problem description that "secret.length() == guess.length()".
      */
      int len = secret.length();
      int bullsCount = 0, cowsCount = 0;
      int cowsCandidatesFromSecret[256] = {0};
      int cowsCandidatesFromGuess[256] = {0};

      for (int i = 0; i < len; ++i) {
        if (secret[i] == guess[i]) {
          ++bullsCount;
        } else {
          if (cowsCandidatesFromSecret[guess[i]] > 0) {
            ++cowsCount;
            --cowsCandidatesFromSecret[guess[i]];
          } else {
            ++cowsCandidatesFromGuess[guess[i]]; // for later matching
          }
          
          if (cowsCandidatesFromGuess[secret[i]] > 0) {
            ++cowsCount;
            --cowsCandidatesFromGuess[secret[i]]; 
          } else {
            ++cowsCandidatesFromSecret[secret[i]]; // for later matching
          }
        }
      }
      
      string ans = to_string(bullsCount) + "A" + to_string(cowsCount) + "B";
      return ans;
    }
};
