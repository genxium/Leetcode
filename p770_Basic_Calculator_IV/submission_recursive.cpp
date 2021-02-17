bool const debug = false;

int const PRIORITY_EXPRESSION = 0;
int const PRIORITY_FACTOR = 1;
int const PRIORITY_TERM = 2;

typedef map<string, int> VAR_POW_T; // "variable -> power"
struct VarPowHash {
    size_t operator() (VAR_POW_T const &varPow) const {
        size_t toRet = 0;
        for (auto &[var, pow] : varPow) {
            toRet += (hash<string>()(var)^pow);
        }
        return toRet;
    }
};

struct VarPowEqual {
    bool operator()(VAR_POW_T const& lhs, VAR_POW_T const& rhs) const {
        for (auto &it : lhs) {
            if (rhs.at(it.first) != it.second) return false;
        }
        for (auto &it : rhs) {
            if (lhs.at(it.first) != it.second) return false;
        }
        return true;
    }
};

typedef unordered_map<VAR_POW_T, int, VarPowHash, VarPowEqual> RT; // a.k.a. "ResultType", i.e. "varPow -> constant coefficient"
void debugPrint(RT const& result) {
    for (auto &[varPow, coef] : result) {
        printf("+%d", coef);
        for (auto &[var, pow] : varPow) {
            printf("*pow(%s, %d)", var.c_str(), pow);
        }
    }
}

vector<pair<int, VAR_POW_T>> toPrintBuff(RT const& result) {
    vector<pair<int, VAR_POW_T>> buff;
    for (auto &[varPow, coef] : result) {
        if (0 == coef) continue;
        buff.push_back({coef, varPow});
    }

    sort(buff.begin(), buff.end(), [](pair<int, VAR_POW_T> const& lhs, pair<int, VAR_POW_T> const& rhs) {
        int lDegree = 0;
        vector<string> lStr;
        for (auto &[var, pow] : lhs.second) {
            lDegree += pow;
            if (pow > 0) {
                for (int i = 0; i < pow; ++i) lStr.push_back(var);
            }
        }

        int rDegree = 0;
        vector<string> rStr;
        for (auto &[var, pow] : rhs.second) {
            rDegree += pow;
            if (pow > 0) {
                for (int i = 0; i < pow; ++i) rStr.push_back(var);
            }
        }

        if (lDegree != rDegree) return lDegree > rDegree;
        return less<vector<string>>()(lStr, rStr);
    });

    return buff;
}

/**
 * The following rules are of left-associative evaluation.
 * 
 * exp := |term
 * 	|term [+ OR -] term 
 * 	|term [+ OR -] term [+ OR -] term ... 
 *
 * term := |factor
 * 	|factor * factor  
 * 	|factor * factor * factor ...  
 *
 * factor := |number
 *  |variable
 * 	|(exp)
 *
 * It's intended that regular-expression notation is NOT used to describe the rules.
 *
 * The introduction of "variable" makes it impossible to return just an "integer" for each rule, thus a uniformed "ResultType" is proposed here.
 * */
RT add(RT& lhs, RT& rhs, bool isSubtraction) {
    RT result;
    for (auto &[lVarPow, lCoef] : lhs) {
        if (rhs.count(lVarPow)) {
            int newCoef = lCoef + (isSubtraction ? -rhs[lVarPow] : rhs[lVarPow]);
            if (0 == newCoef) continue;
            result[lVarPow] = newCoef;
        } else {
            result[lVarPow] = lCoef;
        }
    }

    for (auto &[rVarPow, rCoef] : rhs) {
        if (lhs.count(rVarPow)) {
            continue;
        } else {
            result[rVarPow] = (isSubtraction ? -rCoef : rCoef);
        }
    }

    return result;
}

RT multiply(RT& lhs, RT& rhs) {
    RT result;
    for (auto &[lVarPow, lCoef] : lhs) {
        for (auto &[rVarPow, rCoef] : rhs) {
            VAR_POW_T newVarPow;
            for (auto &[lVar, lPow] : lVarPow) {
                newVarPow[lVar] += lPow;
            }
            for (auto &[rVar, rPow] : rVarPow) {
                newVarPow[rVar] += rPow;
            }
            result[newVarPow] += lCoef*rCoef;
        }
    }

    // Remove those with "coef == 0".
    for (auto it = result.begin(); it != result.end(); ) {
        if (0 == it->second) {
            it = result.erase(it);
        } else {
            ++it;
        }
    }

    return result;
}

bool isValidVarName(string &token) {
    return isalpha(token[0]);
}

RT leftAssociativeEval(vector<string> &tokens, vector<string>::iterator &itToken, int priority) {
	RT ret, tmp;
	string binaryOp = "";
	switch (priority) {
		case PRIORITY_EXPRESSION: {
			// evaluate the first term
			ret = leftAssociativeEval(tokens, itToken, PRIORITY_TERM);
            while (itToken != tokens.end() && ("-" == *itToken || "+" == *itToken)) {
                binaryOp = *itToken;
                ++itToken;
                tmp = leftAssociativeEval(tokens, itToken, PRIORITY_TERM);
                ret = add(ret, tmp, ("-" == binaryOp));
            }
			break;
		}
		case PRIORITY_TERM: {
			// evaluate the first factor 
			ret = leftAssociativeEval(tokens, itToken, PRIORITY_FACTOR);
            while (itToken != tokens.end() && "*" == *itToken) {
                ++itToken;
                tmp = leftAssociativeEval(tokens, itToken, PRIORITY_FACTOR);
                ret = multiply(ret, tmp);
            }
			break;	
		}
		case PRIORITY_FACTOR: {
            if (*itToken == "(") {
                ++itToken;
                ret = leftAssociativeEval(tokens, itToken, PRIORITY_EXPRESSION);
                ++itToken;
            } else {
                if (isValidVarName(*itToken)) {
                    VAR_POW_T newVarPow;
                    newVarPow[*itToken] = 1;
                    ret[newVarPow] = 1;
                } else {
                    int newCoef = stoi(*itToken);
                    VAR_POW_T newVarPow; // Having 0 size as the special init value
                    ret[newVarPow] = newCoef;
                }
                ++itToken;
            }
			break;
		}
	}
	return ret;
}

/*
test cases
"e + 8 - a + 5"
["e"]
[1]
"e - 8 + temperature - pressure"
["e", "temperature"]
[1, 12]
"(e + 8) * (e - 8)"
[]
[]
"7 - 7"
[]
[]
"a * b * c + b * a * c * 4"
[]
[]
"((a - b) * (b - c) + (c - a)) * ((a - b) + (b - c) * (c - a))"
[]
[]
*/
class Solution {
private:
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

        // wrap up
        if (!buff.empty()) {
            toRet.push_back(buff);
        }
        
        return toRet;
    }

public:
    vector<string> basicCalculatorIV(string expression, vector<string>& evalvars, vector<int>& evalints) {
        unordered_set<char> byteDelimitersToInclude = {'(', ')', '+', '-', '*'};
        unordered_set<char> byteDelimitersToExclude = {' ', '\t'};
        vector<string> tokens = tokenize(expression, byteDelimitersToInclude, byteDelimitersToExclude);

        unordered_map<string, string> vars;
        for (int i = 0; i < evalvars.size(); ++i) {
            vars[evalvars[i]] = to_string(evalints[i]);
        }

        for (auto & t : tokens) {
            if (vars.count(t)) {
                t = vars[t];
            }
        }

        if (debug) {
            for (auto & t : tokens) {
                printf("%s,", t.c_str());
            }
            puts("END");
        }

        vector<string>::iterator itToken = tokens.begin();
        RT result = leftAssociativeEval(tokens, itToken, PRIORITY_EXPRESSION);
        if (debug) {
            debugPrint(result);
        }

        vector<pair<int, VAR_POW_T>> printBuffForAns = toPrintBuff(result);
        vector<string> ans;
        for (int i = 0; i < printBuffForAns.size(); ++i) {
            auto &[coef, varPow] = printBuffForAns[i];
            string local = to_string(coef);
            for (auto &[var, pow] : varPow) {
                if (pow > 0) {
                    for (int i = 0; i < pow; ++i) local.append("*" + var);
                }
            }
            ans.push_back(local);
        }
        return ans;
    }
};
