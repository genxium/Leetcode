#define ULL uint64_t
#define LL int64_t

vector<char> plusminus{'+', '-'};

class Solution {
public:
  LL computeVal(LL valueExceptSeries, char lastPlusMinus, LL lastClosedSeries, bool lastOpIsPlusMinus, LL lastOperand) {
    //printf("\tcomputeVal(valueExceptSeries:%ld, lastPlusMinus:%c, lastClosedSeries:%ld, lastOpIsPlusMinus:%d, lastOperand:%ld) == finalValue:", valueExceptSeries, lastPlusMinus, lastClosedSeries, lastOpIsPlusMinus, lastOperand);

    LL finalValue = valueExceptSeries;
    if (false == lastOpIsPlusMinus) {
      lastClosedSeries *= lastOperand;
    } else {
      lastClosedSeries = lastOperand;
    }
    switch(lastPlusMinus) {
      case '+':
        finalValue += lastClosedSeries;
        break;
      case '-':
        finalValue -= lastClosedSeries;
        break;
    }
    //printf("%ld\n", finalValue);
    return finalValue;
  }
  
  bool hasInvalidLeadingZero(string& exp) {
    if ('0' == exp[0] && 1 < exp.length()) {
      return true;
    }
    return false;
  }
  
  void search(string &num, int i, int origLen, string& exp, LL valueExceptSeries, char lastPlusMinus, LL lastClosedSeries, bool lastOpIsPlusMinus, LL lastOperand, vector<string> &ans, int target) {
    //printf("search i:%d, exp:%s, valueExceptSeries:%ld, lastPlusMinus:%c, lastClosedSeries:%ld, lastOpIsPlusMinus:%ld, lastOperand:%ld\n", i, exp.c_str(), valueExceptSeries, lastPlusMinus, lastClosedSeries, lastOpIsPlusMinus, lastOperand);
    /*
    1180+9-8*6*7-0*6+1       +         42
    ------------------       ^        [
    valueExceptSeries      lastPM      lastOperand
    
    
    1180+9-8*6*7-0*6+1+423   -          7*775        *  463
    ----------------------   ^          ~~~~~        ^ [
    valueExceptSeries      lastPM  lastClosedSeries     lastOperand
    */
    if (i == origLen) {
      LL finalValue = computeVal(valueExceptSeries, lastPlusMinus, lastClosedSeries, lastOpIsPlusMinus, lastOperand);
      if (finalValue == (LL)target) {
        ans.push_back(exp.substr(0, exp.length())); 
      }
      return;
    }
    
    LL digitVal = (LL)(num[i]-'0');
    // no-op
    if (0 == i || (0 < lastOperand)) {
      /*
      - don't allow no-op on "lastOperand with leading zero"
      */
      LL newLastOperand = 10*lastOperand + digitVal;
      exp.push_back(num[i]);
      search(num, i+1, origLen, exp, valueExceptSeries, lastPlusMinus, lastClosedSeries, lastOpIsPlusMinus, newLastOperand, ans, target);
      exp.pop_back(); // backtracking
    }
    
    if (0 < i) {
      /*
      - don't insert an operator at the very beginning
      */
      // '+' or '-'
      for (auto &pm : plusminus) {
        LL newValueExceptSeries = computeVal(valueExceptSeries, lastPlusMinus, lastClosedSeries, lastOpIsPlusMinus, lastOperand);
        LL newLastOperand = digitVal;
        exp.push_back(pm); exp.push_back(num[i]);
        search(num, i+1, origLen, exp, newValueExceptSeries, pm, 1, true, newLastOperand, ans, target);
        exp.pop_back(); exp.pop_back(); // backtracking
      }

      // '*'
      if (true == lastOpIsPlusMinus) {
        lastClosedSeries = lastOperand;        
      } else {
        lastClosedSeries *= lastOperand;
      }
      lastOperand = digitVal;
      exp.push_back('*'); exp.push_back(num[i]);
      search(num, i+1, origLen, exp, valueExceptSeries, lastPlusMinus, lastClosedSeries, false, lastOperand, ans, target);
      exp.pop_back(); exp.pop_back(); // backtracking
    }
  }
  
  vector<string> addOperators(string num, int target) {
    /*
    test case #1
    "105"
    5
    
    test case #2
    "3456237490"
    9191
    
    test case #3
    "2147483648"
    -2147483648
    */
    vector<string> ans;
    string exp = "";
    LL valueExceptSeries = (LL)0;
    char lastPlusMinus = '+'; // it doesn't hurt to prepend a '+' to the whole expression
    LL lastClosedSeries = (LL)1;
    bool lastOpIsPlusMinus = false; // bcz it's assumed to be '+' hereby 
    LL lastOperand = (LL)0;
    search(num, 0, num.length(), exp, valueExceptSeries, lastPlusMinus, lastClosedSeries, lastOpIsPlusMinus, lastOperand, ans, target);
    return ans;
  }
};
