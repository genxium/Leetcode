bool debug = false;
int const MAXN = 10000;
int const MAXCH = 26;
int const INVALID = -1;
int nextPos[MAXN+1][MAXCH];

/*
test cases
"ABC"
"ABA"
"LEETCODE"
*/
class Solution {
public:
    int uniqueLetterString(string s) {
        /*
        The key idea is that "countUniqueChars(str)" can be decomposed per-character, i.e. "countUniqueChars(str) = isUnique(str, 'A') + isUnique(str, 'B') + ... + isUnique(str, 'Z')".

        One more thing to notice is that if "isUnique(s[l, ..., r], 'A') == false" is already held, then "isUnique(s[l, ..., >r], 'A') == false". Therefore, see "all substrings" as "all prefixes of all suffixes" (vice-versa is true but not useful in this problem), the answer can be decomposed like

        countUniqueChars(s) = countUniqueChars(s[0, ..., n))
                            = sum{countUniqueCharsInAllPrefixes(s[l, ..., n)) | 0 <= l < n}

        To figure out "countUniqueCharsInAllPrefixes(s[l, ..., n))" for a certain "l", denote "nextPos[l][ch]" to be the first position of character 'ch' in "s[l, ..., n)", then start with "s[l, l]" and trivially "nextPos[l][s[l]] == l". It can be seen that we'd only have "isUnique(s[l, ..., r], 'A') == true" for "nextPos[l][A] <= r < nextPos[nextPos[l][A]+1][A]" for ALL the prefixes of "s[l, ..., n)".
        */

        int n = s.length();
        memset(nextPos, INVALID, sizeof nextPos);
        for (char ch = 'A'; ch <= 'Z'; ++ch) nextPos[n][ch-'A'] = n;
        for (int l = n-1; l >= 0; --l) {
            for (char ch = 'A'; ch <= 'Z'; ++ch) {
                int chIdx = ch-'A';
                if (s[l] == ch) nextPos[l][chIdx] = l;
                else nextPos[l][chIdx] = nextPos[l+1][chIdx];
            }
        }

        int ans = 0;
        for (int l = 0; l < n; ++l) {
            for (char ch = 'A'; ch <= 'Z'; ++ch) {
                int chIdx = ch-'A';
                if (nextPos[l][chIdx] == n) continue;
                int np = nextPos[l][chIdx];
                ans += nextPos[np+1][chIdx]-np;
            }
        }

        return ans;
    }
};
