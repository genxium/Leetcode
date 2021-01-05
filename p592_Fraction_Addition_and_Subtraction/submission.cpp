bool const debug = false;
typedef pair<int, int> PII;

/*
test cases
"-1/2+1/2"
"-1/2+1/2+1/3"
"1/3-1/2"
"5/3+1/3"
*/
class Solution {
private:
    int stoiSafe(string& s) {
        if (s.empty()) return 1;
        return stoi(s);
    }
    
public:
    int gcd(int a, int b) {
        if (a > b) swap(a, b);
        if (0 == a) return b;
        return gcd(b%a, a);
    }
    
    PII addFrac(PII lhs, PII rhs) {
        int d1 = lhs.second, d2 = rhs.second;
        int n1 = lhs.first, n2 = rhs.first;
        
        int dGcd = gcd(d1, d2);
        int enlargedD = (d1/dGcd)*(d2/dGcd)*dGcd;
        return {enlargedD*n1/d1 + enlargedD*n2/d2, enlargedD};
    }
    
    PII subFrac(PII lhs, PII rhs) {
        int d1 = lhs.second, d2 = rhs.second;
        int n1 = lhs.first, n2 = rhs.first;
        
        int dGcd = gcd(d1, d2);
        int enlargedD = (d1/dGcd)*(d2/dGcd)*dGcd;
        return {enlargedD*n1/d1 - enlargedD*n2/d2, enlargedD};
    }
    
    string fractionAddition(string expression) {
        if (expression.empty()) return "";
        
        PII ans = {0, 0};
        int cachedNumerator = 0;
        string buff = "";
        char prevOp = '\0';
        for (auto ch : expression) {
            switch (ch) {
                case '+':
                case '-': {
                    PII tmp = {cachedNumerator, stoiSafe(buff)};
                    if (debug) printf("Got a factor {%d, %d}\n", tmp.first, tmp.second);
                    if (0 == ans.second) ans = tmp;
                    else ans = (prevOp == '+' ? addFrac(ans, tmp) : subFrac(ans, tmp)); 
                    prevOp = ch;
                    buff = "";    
                }
                break;
                case '/': {
                    cachedNumerator = stoiSafe(buff);
                    buff = "";
                }
                break;    
                default: {
                    buff.push_back(ch);                    
                }
                break;
            }
        }
        
        // wrap up
        if (!buff.empty()) {
            PII tmp = {cachedNumerator, stoiSafe(buff)};
            if (debug) printf("Got a factor {%d, %d}\n", tmp.first, tmp.second);
            if (0 == ans.second) ans = tmp;
            else ans = (prevOp == '+' ? addFrac(ans, tmp) : subFrac(ans, tmp));     
        }
        
        if (debug) printf("pre-reduction ans is {%d, %d}\n", ans.first, ans.second);
        int dGcd = gcd(abs(ans.first), abs(ans.second));
        ans.first /= dGcd;
        ans.second /= dGcd;
        
        string ansStr = to_string(ans.first) + "/" + to_string(ans.second);
        return ansStr;
    }
};
