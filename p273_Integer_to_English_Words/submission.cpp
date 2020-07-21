vector<string> ones = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
vector<string> tenToTwenty = {"", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
vector<string> tens = {"", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

vector<string> thousands = {"", "Thousand", "Million", "Billion", "Trillion"};

class Solution {
public:
  string valUnderThousandToWord(int val) {
    vector<int> cache(3, 0);
    int cursor = 0;
    while (val > 0) {
      int r = val%10;
      cache[cursor++] = r;
      val /= 10;
    }
    list<string> words{};
    
    if (cache[0] == 0) {
      if (cache[1] == 0) {
        // do nothing
      } else {
        words.push_back(tens[cache[1]]);
      }
    } else {
      // cache[0] > 0
      if (cache[1] == 0) {
        words.push_back(ones[cache[0]]);
      } else if (cache[1] == 1) {
        words.push_back(tenToTwenty[cache[0]]);
      } else {
        words.push_back(ones[cache[0]]);
        words.push_back(tens[cache[1]]);
      }
    }
    
    if (cache[2] > 0) {
      words.push_back("Hundred");
      words.push_back(ones[cache[2]]);
    }
    
    string toRet = "";
    for (auto it = words.rbegin(); it != words.rend(); ++it) {
      if (it != words.rbegin()) toRet.push_back(' ');
      toRet.append((*it));
    }
    return toRet;
  }
  
  string numberToWords(int num) {
    /*
    test case #1
    1234567891
    
    test case #2
    1204560091
    
    test case #3
    0204560091
    
    test case #4
    2147483647
    
    test case #5
    1000
    
    test case #6
    1000000
    */
    if (0 == num) {
      return "Zero";
    }
    list<string> words{};
    string ans = "";
    int thousandsCount = 0;
    while (num > 0) {
      int r = num%1000;
      if (0 < r) {
        string word = valUnderThousandToWord(r);
        if (thousandsCount > 0) {
          words.push_back(thousands[thousandsCount]);
        }
        words.push_back(word);        
      }
      num /= 1000;
      ++thousandsCount;
    }
    for (auto it = words.rbegin(); it != words.rend(); ++it) {
      if (it != words.rbegin()) ans.push_back(' ');
      ans.append((*it));
    }
    return ans;
  }
};
