bool cmp(string const &lhs, string const &rhs) {
    if (lhs.length() != rhs.length()) return lhs.length() > rhs.length();
    int r = lhs.compare(rhs);
    return r < 0;
}

bool contains(string &lhs, string &rhs) {
    if (lhs.length() < rhs.length()) return false;
    int lCur = 0, rCur = 0;
    while (rCur < rhs.length()) {
        while (lCur < lhs.length() && lhs[lCur] != rhs[rCur]) ++lCur;
        if (lCur < lhs.length()) {
            ++lCur;
            ++rCur;
        } else {
            break;
        }
    }
    return rCur == rhs.length();
}

class Solution {
public:
    string findLongestWord(string s, vector<string>& d) {
        sort(d.begin(), d.end(), &cmp);
        for (auto &single : d) {
            if (contains(s, single)) return single;
        }
        return "";
    }
};
