bool const debug = false;
typedef map<string, int> COUNTER_T;

/*
[WARNING] If the input formula were not always valid, we should maintain a "FiniteStateAutomata" for the parsing, like https://github.com/genxium/Leetcode/tree/master/p591_Tag_Validator.
*/

/*
"H2O"
"Mg(OH)2"
"K4(ON(SO3)2)2"
"Be32"
*/
class Solution {
public:
    void parse(string &formula, int &i, COUNTER_T &counter, int level) {
        if (i == formula.length()) return;
        int indent = 2*level;
        if (debug) printf("%*sparse i:%d\n", indent, "", i);
        while (i < formula.length()) {
            COUNTER_T localCounter;
            if (formula[i] == ')') {
                ++i;
                return; // pops from the callstack
            } else if (formula[i] == '(') {
                ++i;
                parse(formula, i, localCounter, level+1);
                if (debug) printf("%*sfinished parsing an EmbededFormula, now at formula[i:%d]:%c\n", indent, "", i, formula[i]);        
            } else {
                // Read a primitive EleName
                string buff = "";
                buff.push_back(formula[i++]);    
                while (i < formula.length() && isalpha(formula[i]) && formula[i] > 'Z') {
                    buff.push_back(formula[i++]);
                }
                localCounter[buff] = 1;
                if (debug) printf("%*sfinished parsing a primitive EleName, now at formula[i:%d]:%c\n", indent, "", i, formula[i]);        
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
            for (auto it : localCounter) {
                counter[it.first] += it.second*localCountMul;
            }
        }
    }

    string countOfAtoms(string formula) {
        int i = 0;
        COUNTER_T counter;
        parse(formula, i, counter, 0);
        string ans = "";
        for (auto it : counter) {
            ans += it.first;
            if (it.second > 1) ans += to_string(it.second);
        }
        return ans;
    }
};
