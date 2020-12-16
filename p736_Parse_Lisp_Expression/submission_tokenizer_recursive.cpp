int const LET_STATE_EXPECTING_VAR_NAME = 0;
int const LET_STATE_EXPECTING_VAR_VAL = 1;
int const LET_STATE_LOCAL_EXP_EVALED = 2;
bool const debug = false;

/*
test cases
"(add 1 2)"
"(mult 3 (add 2 3))"
"(let x 2 (mult x 5))"
"(let x 2 (mult x (let x 3 y 4 (add x y))))"
"(let x 3 x 2 x)"
"(let x 1 y 2 x (add x y) (add x y))"
"(let x 2 (add (let x 3 (let x 4 x)) x))"
"(let a1 -3 b2 (add a1 1) b2)" 
*/
class Solution {
private:
    bool isValidVarName(string &token) {
      return isalpha(token[0]);
    }

    int canEvalValidVarName(string &token, unordered_map<string, int> &variables) {
      return isValidVarName(token) && variables.find(token) != variables.end();
    }

public:
    vector<string> tokenize(string &str, unordered_set<char> &byteDelimitersToInclude, unordered_set<char> &byteDelimitersToExclude) {
        char const * tmpPtr = str.c_str();		
        vector<string> toRet;
        
        string buff = "";
        while (*tmpPtr != '\n' && *tmpPtr != '\0') {
            if (byteDelimitersToInclude.find(*tmpPtr) != byteDelimitersToInclude.end()) {
                if (0 < buff.length()) {
                    string t(buff);
                    toRet.push_back(t);
                    buff = "";
                }
                string t = ""; t.push_back(*tmpPtr);
                toRet.push_back(t);
            } else if (byteDelimitersToExclude.find(*tmpPtr) != byteDelimitersToExclude.end()) {
                if (0 < buff.length()) {
                    string t(buff);
                    toRet.push_back(t);
                    buff = "";
                }
            } else {
                buff.push_back(*tmpPtr);
            }
            ++tmpPtr;
        }

        return toRet;
    }

    /**
    * The following rules are of left-associative evaluation.
    * 
    * exp := |factor
    * 	|add exp exp
    * 	|mult exp exp
    *   |let var exp [var exp]... exp
    *
    * factor := |number
    *   |var
    * 	|(exp)
    *
    * It's intended that regular-expression notation is NOT used to describe the rules.
    * */

    int underlyingEvalFactor(int &cur, vector<string> &tokens, unordered_map<string, int> &variables, int level) {
        int indent = 2*level;
        if ('(' == tokens[cur][0]) {
            if (debug) printf("%*sevaluating (exp) factor at cur:%d\n", indent, "", cur);
            ++cur;
            int number = underlyingEvalExp(cur, tokens, variables, level+1);
            ++cur; // At ')' and pass by 
            if (debug) printf("%*sevaluated (exp) factor at cur:%d, returning number:%d\n", indent, "", cur, number);
            return number;
        } else if (isValidVarName(tokens[cur])) {
            int number = variables[tokens[cur]];
            if (debug) printf("%*sevaluating var factor at cur:%d, returning number:%d\n", indent, "", cur, number);
            ++cur;
            return number; 
        } else {
            int number = stoi(tokens[cur]);
            if (debug) printf("%*sevaluating number factor at cur:%d, returning number:%d\n", indent, "", cur, number);
            ++cur;
            return number; 
        }
    }

    int underlyingEvalExp(int &cur, vector<string> &tokens, unordered_map<string, int> &variables, int level) {
        int indent = 2*level;
        if (0 == tokens[cur].compare("add")) {
            if (debug) printf("%*sstart processing `add` at cur:%d\n", indent, "", cur);
            ++cur;
            int lExp = underlyingEvalExp(cur, tokens, variables, level+1);
            int rExp = underlyingEvalExp(cur, tokens, variables, level+1);
            int addResult = lExp+rExp;
            if (debug) printf("%*send processing `add` at cur:%d, returning addResult:%d\n", indent, "", cur, addResult);
            return addResult;
        } else if (0 == tokens[cur].compare("mult")) {
            if (debug) printf("%*sstart processing `mult` at cur:%d\n", indent, "", cur);
            ++cur;
            int lExp = underlyingEvalExp(cur, tokens, variables, level+1);
            int rExp = underlyingEvalExp(cur, tokens, variables, level+1);
            int multResult = lExp*rExp;
            if (debug) printf("%*send processing `mult` at cur:%d, returning multResult:%d\n", indent, "", cur, multResult);
            return multResult;
        } else if (0 == tokens[cur].compare("let")) {
            if (debug) printf("%*sstart processing `let` at cur:%d\n", indent, "", cur);
            unordered_map<string, int> origVariables;
            for (auto it : variables) {
                origVariables[it.first] = it.second;
            }
            ++cur;
            int letState = LET_STATE_EXPECTING_VAR_NAME;
            string activeVarName = "";
            int activeVarVal = 0, letResult = 0;
            while (letState != LET_STATE_LOCAL_EXP_EVALED) {
                switch (letState) {
                    case LET_STATE_EXPECTING_VAR_NAME:  
                        if (isValidVarName(tokens[cur])) {
                            // Definitely a new variable
                            activeVarName = tokens[cur];
                            ++cur;
                            if (')' == tokens[cur][0]) {
                                letResult = variables[activeVarName];
                                if (debug) printf("%*s\t#2 got letResult:%d, now cur:%d\n", indent, "", letResult, cur);
                                letState = LET_STATE_LOCAL_EXP_EVALED; 
                            } else {
                                letState = LET_STATE_EXPECTING_VAR_VAL; 
                            }
                            if (debug) printf("%*s\tgot activeVarName:%s, now cur:%d, letState:%d\n", indent, "", activeVarName.c_str(), cur, letState);
                        } else {
                            // Definitely the last "LOCAL_EXP"
                            letResult = underlyingEvalExp(cur, tokens, variables, level+1); 
                            letState = LET_STATE_LOCAL_EXP_EVALED;
                            if (debug) printf("%*s\t#1 got letResult:%d, now cur:%d\n", indent, "", letResult, cur); 
                        }
                    break; 
                    case LET_STATE_EXPECTING_VAR_VAL:  
                        activeVarVal = underlyingEvalExp(cur, tokens, variables, level+1);
                        variables[activeVarName] = activeVarVal;
                        letState = LET_STATE_EXPECTING_VAR_NAME;
                        if (debug) printf("%*s\tgot activeVarName:%s -> activeVarVal:%d, now cur:%d, letState:%d\n", indent, "", activeVarName.c_str(), activeVarVal, cur, letState);
                    break; 
                    default:  
                    break; 
                }
            }
            variables.clear(); // backtrace
            for (auto it : origVariables) {
                variables[it.first] = it.second;
            }
            if (debug) printf("%*send processing `let` at cur:%d\n", indent, "", cur);
            return letResult;
        } else {
            // Single factor
            return underlyingEvalFactor(cur, tokens, variables, level);
        }
    }

    int evaluate(string s) {
        /*
        References
        - https://github.com/genxium/SimpleTokenizer
        - https://github.com/genxium/Leetcode/tree/master/p224_Basic_Calculator/
        */
        unordered_set<char> byteDelimitersToInclude = {'(', ')'};
        unordered_set<char> byteDelimitersToExclude = {' ', '\t'};
        vector<string> tokens = tokenize(s, byteDelimitersToInclude, byteDelimitersToExclude);
        if (debug) {
            for (auto & t : tokens) {
                printf("%s,", t.c_str());
            }
            puts("END");
        }
        unordered_map<string, int> variables;
        int cur = 0;
        return underlyingEvalExp(cur, tokens, variables, 0);
    }
};

