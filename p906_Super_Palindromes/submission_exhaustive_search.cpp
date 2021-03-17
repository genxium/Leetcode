bool debug = false;
typedef uint64_t ULL;

/*
test cases
"4"
"1000"
"1"
"2"
*/
class Solution {
public:
    bool isPalindromeStr(string &s) {
        int l = 0, r = s.length()-1;
        while (l <= r) {
            if (s[l] != s[r]) return false;
            ++l;
            --r;
        }

        return true;
    }

    bool isPalindrome(ULL x) {
        string s = to_string(x);
        return isPalindromeStr(s);
    }

    void genRoot(int len /* expected length */, int i, vector<string> &roots, string &buff) {
        // A "test run" for "root in [1, 1000000]" suggests that a "root" of length larger than 1 can only have digits among {0, 1, 2}.
        for (int d = 0; d <= 2; ++d) {
            if (d == 0 && i == 0) continue; // don't start with leading 0
            buff.push_back('0'+d);

            if ((len & 1) && i == (len >> 1)) {
                // The expected length of root is odd, and we reaches half
                string u = buff;
                string v = buff; v.pop_back(); reverse(v.begin(), v.end());
                u.append(v);
                ULL cand = stoull(u);
                ULL x = cand*cand;
                if (isPalindrome(x)) {
                    roots.push_back(u);
                }
            } else if (!(len & 1) && i+1 == (len >> 1)) {
                // The expected length of root is even, and we reaches half
                string u = buff;
                string v = buff; reverse(v.begin(), v.end());
                u.append(v);
                ULL cand = stoull(u);
                ULL x = cand*cand;
                if (isPalindrome(x)) {
                    roots.push_back(u);
                }
            } else {
                genRoot(len, i+1, roots, buff);
            }

            buff.pop_back(); // backtrace
        }
    }

    int superpalindromesInRange(string left, string right) {
        string buff = "";
        vector<string> roots;
        // root with length 1 can only be 1, 2, 3, thus having super palindromes 1, 4, 9
        roots.push_back("1");
        roots.push_back("2");
        roots.push_back("3"); 
        for (int len = 2; len <= 9; ++len) {
            genRoot(len, 0, roots, buff);
        }

        int ans = 0;
        ULL leftU = stoull(left), rightU = stoull(right);
        for (auto &root : roots) {
            ULL cand = stoull(root); cand *= cand;
            if (leftU <= cand && cand <= rightU) ++ans;
            if (debug) {
                printf("root: %s, the SuperPalindrome is %llu\n", root.c_str(), cand);
            }
        }

        return ans;
    }
};
