/**
 * // This is the interface that allows for creating nested lists.
 * // You should not implement it, or speculate about its implementation
 * class NestedInteger {
 *   public:
 *     // Constructor initializes an empty nested list.
 *     NestedInteger();
 *
 *     // Constructor initializes a single integer.
 *     NestedInteger(int value);
 *
 *     // Return true if this NestedInteger holds a single integer, rather than a nested list.
 *     bool isInteger() const;
 *
 *     // Return the single integer that this NestedInteger holds, if it holds a single integer
 *     // The result is undefined if this NestedInteger holds a nested list
 *     int getInteger() const;
 *
 *     // Set this NestedInteger to hold a single integer.
 *     void setInteger(int value);
 *
 *     // Set this NestedInteger to hold a nested list and adds a nested integer to it.
 *     void add(const NestedInteger &ni);
 *
 *     // Return the nested list that this NestedInteger holds, if it holds a nested list
 *     // The result is undefined if this NestedInteger holds a single integer
 *     const vector<NestedInteger> &getList() const;
 * };
 */
class Solution {
private:
    int cursor = 0;
    NestedInteger* parse(string& s) {
        NestedInteger* toRet = new NestedInteger();
        if (isdigit(s[cursor]) || '-' == s[cursor]) {
            string valStr = "";
            if ('-' == s[cursor]) {
                valStr.push_back(s[cursor]);
                ++cursor;
            }
            while (cursor < s.length() && isdigit(s[cursor])) {
                valStr.push_back(s[cursor]);
                ++cursor;
            }
            int val = atoi(valStr.c_str());
            toRet->setInteger(val);
        } else {
            /* assuming the expression is always valid, we should always have 
            ``` 
            '[' == s[cursor]
            ```
            in this else-branch
            */
            if ('[' == s[cursor]) {
                ++cursor;
                while (cursor < s.length() && ']' != s[cursor]) {
                    NestedInteger* nextEle = parse(s);
                    // printf("in list, nextEle parsed, cursor == %d\n", cursor);
                    toRet->add((*nextEle));
                    if (',' == s[cursor]) ++cursor;
                }
                // definitely arrives at ']', assuming the expression is always valid
                ++cursor; // passes ']'                    
            }
        }
        return toRet;
    }
public:
    NestedInteger deserialize(string s) {
        NestedInteger* pAns = parse(s);
        return *pAns;
    }
};
