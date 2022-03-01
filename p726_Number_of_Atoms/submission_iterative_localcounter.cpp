bool const debug = false;
typedef map<string, int> COUNTER_T;

/*
"H2O"
"Mg(OH)2"
"K4(ON(SO3)2)2"
"Be32"
*/
class Solution {
public:
    string countOfAtoms(string formula) {
        COUNTER_T rootCounter;
        int i = 0;
        stack<COUNTER_T> stk;
        stk.push(rootCounter); // Ensure that there's at least 1 elementleft 
        while (i < formula.length()) {
            COUNTER_T localCounter;
            if (formula[i] == '(') {
                stk.push(localCounter);
                ++i;
            } else if (formula[i] == ')') {
                for (auto it : stk.top()) {
                    localCounter[it.first] += it.second;
                }
                stk.pop();
                ++i;
                if (debug) printf("finished parsing an EmbededFormula, now at formula[i:%d]:%c\n", i, formula[i]);        
            } else {
                // Read a primitive EleName
                string buff = "";
                buff.push_back(formula[i++]);    
                while (i < formula.length() && isalpha(formula[i]) && formula[i] > 'Z') {
                    buff.push_back(formula[i++]);
                }
                localCounter[buff] = 1;
                if (debug) printf("finished parsing an EleName, now at formula[i:%d]:%c\n", i, formula[i]);        
            }

            // Expecting count multiplier for the just parsed "EmbeddedFormula" or primitive "EleName"
            int localCountMul = 1;
            string buff = "";
            while (i < formula.length() && isdigit(formula[i])) {
                buff.push_back(formula[i++]);
            }
            if (!buff.empty()) {
                localCountMul = stoi(buff);
            }
            COUNTER_T &curTop = stk.top(); 
            for (auto it : localCounter) {
                curTop[it.first] += it.second*localCountMul;
            }
        }
        
        string ans = "";
        for (auto it : stk.top()) {
            ans += it.first;
            if (it.second > 1) ans += to_string(it.second);
        }
        return ans;
    }
};
