bool const debug = false;

string& ltrim(string& str, const string& chars = "0\t\n\v\f\r ") {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

bool isPalindromic(string &s) {
    int mid = (s.length() >> 1);
    for (int i = 0; i < mid; ++i) {
        if (s[i] != s[s.length()-1-i]) return false;
    }
    return true;
}

bool isOneTrailedByAllZeros(string &s) {
    if (s[0] != '1') return false;
    for (int i = 1; i < s.length(); ++i) {
        if(s[i] != '0') return false;
    }
    return true;
}

string diff(string &larger, string &smaller) {
    string res(larger);
    string b(smaller);
    int decr = 0;
    reverse(b.begin(), b.end());
    reverse(res.begin(), res.end());
    for (int i = 0; i < b.length(); ++i) {
        if (b[i] + decr > res[i]) {
            res[i] = res[i] + 10 - (decr + (int)(b[i]-'0'));
            decr = 1;
        } else {
            res[i] -= (decr + (int)(b[i]-'0'));
            decr = 0;
        }
    }

    if (res.length() > b.length()) {
        for (int i = b.length(); i < res.length(); ++i) {
            if (decr > res[i]) {
                res[i] = res[i] + 10 - (decr);
                decr = 1;
            } else {
                res[i] -= (decr);
                decr = 0;
                break;
            }
        }
    }

    reverse(res.begin(), res.end());
    if (res.length() > 1) res = ltrim(res);
    return res;
}

int decimalcompare(string const &lhs, string const &rhs) {
    if (lhs.length() != rhs.length()) return (lhs.length() > rhs.length() ? +1 : -1);
    for (int i = 0; i < lhs.length(); ++i) {
        if (lhs[i] != rhs[i]) return (lhs[i] > rhs[i] ? +1 : -1);
    }
    return 0;
}

bool decimalcomparePss(pair<string, string> const &lhs, pair<string, string> const &rhs) {
    int initial = decimalcompare(lhs.first, rhs.first); 
    if (0 != initial) return (initial < 0);
    int valueComp = decimalcompare(lhs.second, rhs.second);
    return (valueComp < 0);
}

/*
test cases
"1"
"123"
"9998"
"321"
"1000000000"
"1000000001"
"1001001"
"10011001"
"2002"
"20002"
"2112"
"23332"
"11"
"99"
"99999999999"
"99909999999"
"99999099999"
*/
class Solution {
private:
    void listCandidatesOfPalindrome(string& ss, string &s, vector<pair<string, string>> &candidates) {
        string candLow = "";
        for (int i = 0; i < s.length()-1; ++i) {
            candLow.push_back('9');
        }
        string diffFromLow = diff(s, candLow); 
        
        string candHigh = "1";
        for (int i = 0; i < s.length()-1; ++i) {
            candHigh.push_back('0');
        }
        candHigh.push_back('1');
        string diffToHigh = diff(candHigh, s);
        candidates.push_back({diffFromLow, candLow});
        candidates.push_back({diffToHigh, candHigh});
                
        // ss == "2011102"
        string cand0(ss);
        int mid = (ss.length() >> 1), i = 0;
        while (mid+i < ss.length()) {
            if (cand0[mid+i] > '0') {
                --cand0[mid+i];
                if (ss.length()-1-(mid+i) != (mid+i)) {
                    --cand0[ss.length()-1-(mid+i)];
                }
                break;
            }
            cand0[mid+i] = '9';
            cand0[ss.length()-1-(mid+i)] = '9';
            ++i;
        }
        if (cand0[0] == '0' && cand0.length() > 1) cand0 = ltrim(cand0) + "9";
        if (decimalcompare(s, cand0) < 0) {
            candidates.push_back({diff(cand0, s), cand0});
        } else {
            candidates.push_back({diff(s, cand0), cand0});
        }
        
        // ss == "9099909"
        string cand1 = ss;
        i = 0;
        while (mid+i < ss.length()) {
            if (cand1[mid+i] < '9') {
                ++cand1[mid+i];
                if (ss.length()-1-(mid+i) != (mid+i)) {
                    ++cand1[ss.length()-1-(mid+i)];
                }
                break;
            }
            cand1[mid+i] = '0';
            cand1[ss.length()-1-(mid+i)] = '0';
            ++i;
        }
        if (cand1[0] == '0' && cand1.back() == '1') cand1 = "1" + cand1;
        else cand1 = ltrim(cand1);
        if (decimalcompare(s, cand1) < 0) {
            candidates.push_back({diff(cand1, s), cand1});
        } else {
            candidates.push_back({diff(s, cand1), cand1});
        }
    }
    
public:
    string nearestPalindromic(string s) {
        if (1 == s.length()) {
            string ans = s;
            --ans[0];
            return ans;
        }
        vector<pair<string, string>> candidates;
        string ss;
        if (!isPalindromic(s)) {
            int len = s.length(), halfLen = len/2;
            for (int padLen = 1; padLen <= halfLen; ++padLen) {
                string prefix = s.substr(0, len-padLen);
                string suffix = s.substr(0, padLen);
                reverse(suffix.begin(), suffix.end());
                ss = prefix + suffix;
                if (isPalindromic(ss)) break;
            }
            if (decimalcompare(s, ss) < 0) {
                // s < ss
                candidates.push_back({diff(ss, s), ss});
            } else {
                // s > ss
                candidates.push_back({diff(s, ss), ss});
            }
        } else {
            ss = s;
        }
        listCandidatesOfPalindrome(ss, s, candidates);
        if (debug) {
            for (auto &it : candidates) {
                printf("(diff: %s, cand: %s)\n", it.first.c_str(), it.second.c_str());
            }
        }
        sort(candidates.begin(), candidates.end(), &decimalcomparePss);
        pair<string, string> &best = *(candidates.begin());
        if (debug) printf("for s:%s, best is {%s, %s}\n", s.c_str(), best.first.c_str(), best.second.c_str());
        return best.second;
    }
};
