bool const debug = false;

int const INVALID = INT_MIN;

int const TYPE_UNKNOWN = 0;
int const TYPE_LET = 1;
int const TYPE_ADD = 2;
int const TYPE_MULT = 3;

int const LET_STATE_NONE = 0; 
int const LET_STATE_EXPECTING_VAR_NAME = 1;
int const LET_STATE_EXPECTING_VAR_VAL = 2;

class ClosureTuple {
public:
    unordered_map<string, int> variables;
    vector<int> operands;
    int opType;
    string activeVarName; // Used by "TYPE_LET" only.
    int letState;

    ClosureTuple(): opType(TYPE_UNKNOWN),activeVarName(""),letState(LET_STATE_NONE) {}
    ClosureTuple(unordered_map<string, int> newVariables): variables(newVariables),opType(TYPE_UNKNOWN),activeVarName(""),letState(LET_STATE_NONE) {}

    int opResult() {
        switch (opType) {
            case TYPE_ADD:
                return operands[0] + operands[1];
            case TYPE_MULT:
                return operands[0] * operands[1];
            case TYPE_LET:
                if (operands.empty()) return variables[activeVarName];
                return operands.back();
        }

        return INVALID;
    }
};

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
"(let x -2 y x y)"
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

    void feedVarName(string &varName, stack<ClosureTuple> &stk) {
        auto &t = stk.top();
        switch (t.opType) {
            case TYPE_ADD:
            case TYPE_MULT:
                t.operands.push_back(t.variables[varName]);
                break;
            case TYPE_LET:
                if (debug) printf("feeding varName \"%s\" to opType:%d while letState is %d\n", varName.c_str(), t.opType, t.letState);
                if (t.letState == LET_STATE_EXPECTING_VAR_NAME) {
                    t.activeVarName = varName;
                    t.letState = LET_STATE_EXPECTING_VAR_VAL;
                } else {
                    t.variables[t.activeVarName] = t.variables[varName];
                    t.activeVarName = "";
                    t.letState = LET_STATE_EXPECTING_VAR_NAME;
                }
                break;
        }
    }

    void feedNumber(int number, stack<ClosureTuple> &stk) {
        auto &t = stk.top();
        switch (t.opType) {
            case TYPE_ADD:
            case TYPE_MULT:
                t.operands.push_back(number);
                break;
            case TYPE_LET:
                if (t.letState == LET_STATE_EXPECTING_VAR_NAME) {
                    t.operands.push_back(number); // As the "result" when popped.
                    t.letState = LET_STATE_NONE;
                } else {
                    t.variables[t.activeVarName] = number;
                    t.activeVarName = "";
                    t.letState = LET_STATE_EXPECTING_VAR_NAME;
                }
                break;
        }
    }

    int underlyingEval(vector<string> &tokens) {
        stack<ClosureTuple> stk;
        for (int cur = 0; cur < tokens.size(); ++cur) {
            if (debug) printf("cur: %d\n", cur);
            if (tokens[cur] == ")") {
                if (debug) printf("\tpopping\n");
                int holdingVal = stk.top().opResult();
                if (debug) printf("\tgot holdingVal = %d\n", holdingVal);
                stk.pop();
                if (debug) printf("\tpopped\n");
                /*
                [WARNING] There could be consecutive ')'s, thus we use "feedNumber(...)" to cache the result in the next "stk.top()".
                */
                if (stk.empty()) {
                    if (debug) printf("Finished traversing the whole expression\n");
                    return holdingVal;
                }

                feedNumber(holdingVal, stk);
            } else if (tokens[cur] == "(") {
                ClosureTuple newClosure;
                if (!stk.empty()) {                
                    newClosure.variables = stk.top().variables; // a copy
                }
                stk.push(newClosure);
            } else if (tokens[cur] == "add") {
                stk.top().opType = TYPE_ADD;
            } else if (tokens[cur] == "mult") {
                stk.top().opType = TYPE_MULT;
            } else if (tokens[cur] == "let") {
                stk.top().opType = TYPE_LET;
                stk.top().letState = LET_STATE_EXPECTING_VAR_NAME;
            } else {
                // Could only be variable name or number here.
                if (isValidVarName(tokens[cur])) {
                    feedVarName(tokens[cur], stk);
                } else {
                    int number = stoi(tokens[cur]);
                    feedNumber(number, stk);
                }
            }
        }
        return INVALID;
    }

    int evaluate(string s) {
        unordered_set<char> byteDelimitersToInclude = {'(', ')'};
        unordered_set<char> byteDelimitersToExclude = {' ', '\t'};
        vector<string> tokens = tokenize(s, byteDelimitersToInclude, byteDelimitersToExclude);
        if (debug) {
            for (auto & t : tokens) {
                printf("%s,", t.c_str());
            }
            puts("END");
        }
        return underlyingEval(tokens);
    }
};
