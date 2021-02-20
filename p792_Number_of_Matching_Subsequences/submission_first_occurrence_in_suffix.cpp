bool const debug = true;
int const MAXN = 50000, MAXCH = 26, INVALID = -1;
int firstOccurrenceAfter[MAXN+1][MAXCH];

class Solution {
public:
    int numMatchingSubseq(string S, vector<string>& words) {
        memset(firstOccurrenceAfter, INVALID, sizeof firstOccurrenceAfter);

        for (int i = S.length()-1; i >= 0; --i) {
            unordered_map<char, int> dict;
            for (char ch = 'a'; ch <= 'z'; ++ch) {
                int chIdx = ch-'a';
                if (ch == S[i]) {
                    firstOccurrenceAfter[i][chIdx] = i;
                } else {
                    if (i == S.length()-1) firstOccurrenceAfter[i][chIdx] = INVALID;
                    else firstOccurrenceAfter[i][chIdx] = firstOccurrenceAfter[i+1][chIdx];
                }
            }
        }        

        int ans = 0;
        for (auto &word : words) {
            int i = 0, j = 0; // i in "word", j in "S"
            for (; i < word.length(); ++i) {
                char ch = word[i];
                int chIdx = ch-'a';
                if (j < S.length() && INVALID != firstOccurrenceAfter[j][chIdx]) {
                    j = firstOccurrenceAfter[j][chIdx]+1;
                } else {
                    break;
                }
            }

            ans += (i == word.length());
        }

        return ans;
    }
};
