class Solution {
public:
    int compress(vector<char>& chars) {
      int l = chars.size(), slotPos = 0;
      char cur = chars[0], curCount = 1;
      for (int i = 1; i < l; ++i) {
        if (chars[i] != cur) {
          chars[slotPos++] = cur;
          if (curCount > 1) {
            string countString = to_string(curCount);
            for (int j = 0; j < countString.length(); ++j) {
              chars[slotPos++] = countString[j];
            }  
          }
          curCount = 0;
        }
        cur = chars[i];
        ++curCount;
      }
      // wrap up
      chars[slotPos++] = cur;
      if (curCount > 1) {
        string countString = to_string(curCount);
        for (int j = 0; j < countString.length(); ++j) {
          chars[slotPos++] = countString[j];
        }  
      }
      return slotPos;
    }
};
