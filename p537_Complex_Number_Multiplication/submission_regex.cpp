/*
test cases
"1+1i"
"1+1i"
"1+-1i"
"1+-1i"
"1+0i"
"-1+10000i"
"1+-0i"
"1+0i"
*/
bool const debug = false;

class Solution {
private:
    pair<int, int> parseFromCm(cmatch &result) {
        int real = stoi(result.str(1));
        if (2 >= result.size()) return {real, 0};
        if (0 == result.str(2).length()) return {real, 0};
        int img = 1;
        if (result.str(4).length() > 0) {
            img = stoi(result.str(4));
        }
        if ("-" == result.str(3)) img = -img;
        return {real, img};
    }
    
public:
    string complexNumberMultiply(string a, string b) {
        cmatch resultA, resultB;
        
        regex cnRex("([+-]?[0-9]+)(([+-])([+-]?[0-9]+)i)?", regex_constants::ECMAScript | regex_constants::icase);
        int realA = 0, imgA = 0, realB = 0, imgB = 0;
        if (regex_match(a.c_str(), resultA, cnRex)) {
            auto [real, img] = parseFromCm(resultA);
            realA = real;
            imgA = img;
        }
        
        if (regex_match(b.c_str(), resultB, cnRex)) {
            auto [real, img] = parseFromCm(resultB);
            realB = real;
            imgB = img;
        }
        
        if (debug) printf("realA: %d, imgA: %d, realB: %d, imgB: %d\n", realA, imgA, realB, imgB);
        
        int ansReal = realA*realB - imgA*imgB;
        int ansImg = realA*imgB + realB*imgA;
        
        char buf[65535]; memset(buf, 0, sizeof(buf));
        sprintf(buf, "%d+%di", ansReal, ansImg);
        string ans(buf);
        
        return ans;
    }
};
