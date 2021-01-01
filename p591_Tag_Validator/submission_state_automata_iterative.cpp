bool const debug = false;
int const IDLE = 0;
int const EXPECTING_TAG_NAME_EMPTY = 1;
int const EXPECTING_TAG_NAME_MORE = 2;
int const EXPECTING_TAG_CONTENT = 3;
int const EXPECTING_TAG_EMPTY_MIXED = 4;
int const EXPECTING_CDATA_START = 5;
int const EXPECTING_CDATA_CONTENT = 6;
int const EXPECTING_CDATA_ENDIND_1 = 7;
int const EXPECTING_CDATA_ENDIND_2 = 8;
int const EXPECTING_TAG_END_NAME = 9;
int const STACK_PREEMPTIED = 10;
/*
test cases
"<DIV>This is the first line <![CDATA[<div>]]></DIV>"
"<DIV>This is the first line <<![CDATA[<div>]]></DIV>"
"<A>  <B> </A>   </B>"
"<A> <B>SomeContent   </B> </A>"
"<A> <![CDATA[first half]]>  <B>SomeContent   </B> <![CDATA[<<<<>>><<second half>]]>  </A>"
"<TRUe><![CDATA[123123]]]]><![CDATA[>123123]]></TRUe>"
"<A></A><B></B>"
"<A> <![CDATA[first half]]>  <B>SomeContent   </B> <![CDATA[<<<<>>><<second half>]]>  </A>    "
"<A> <![CDATA[first half]]>  <B>SomeContent   </B> <![CDATA[<<<<>>><<second half>]]>  </A>    !  "
*/
class Solution {
private:
    bool isValidTagName(string &activeTagName) {
        if (0 == activeTagName.length() || activeTagName.length() > 9) return false;
        for (auto ch : activeTagName) {
            if (ch < 'A' || ch > 'Z') return false;
        }
        return true;
    }
public:
    bool isValid(string code) {
        int state = IDLE;
        int i = 0, n = code.length();
        
        stack<string> stkActiveTags;
        string activeTagName = "";
        string cdataStarter = "[CDATA[";
        while (i < n) {
            char ch = code[i];
            if (debug) printf("i:%d, state: %d, ch:%c, activeTagName:%s\n", i, state, ch, activeTagName.c_str());
            switch (state) {
                case STACK_PREEMPTIED:
                    return false;
                case IDLE:
                    if (ch != '<') return false;  
                    state = EXPECTING_TAG_NAME_EMPTY;
                    break;
                case EXPECTING_TAG_NAME_EMPTY:
                    if (ch == '<') return false;
                    if (ch == '>') return false;
                    activeTagName.push_back(ch);
                    state = EXPECTING_TAG_NAME_MORE;
                    break;
                case EXPECTING_TAG_NAME_MORE:
                    if (ch == '<') return false;
                    if (ch == '>') {
                        if (false == isValidTagName(activeTagName)) return false;
                        stkActiveTags.push(activeTagName);
                        activeTagName = "";
                        state = EXPECTING_TAG_CONTENT;
                    } else {
                        activeTagName.push_back(ch);
                    }
                    break;
                case EXPECTING_TAG_CONTENT:
                    if (ch == '<') {
                        state = EXPECTING_TAG_EMPTY_MIXED;
                    }
                    break;
                case EXPECTING_TAG_EMPTY_MIXED:
                    if (ch == '<' || ch == '>') return false;
                    if (ch == '!') {
                        state = EXPECTING_CDATA_START;
                    } else if (ch == '/') {
                        state = EXPECTING_TAG_END_NAME;
                    } else {
                        activeTagName.push_back(ch);
                        state = EXPECTING_TAG_NAME_MORE;
                    }
                    break;
                case EXPECTING_CDATA_START:
                    if (i+cdataStarter.length()-1 >= n || 0 != code.substr(i, cdataStarter.length()).compare(cdataStarter)) return false;
                    i += cdataStarter.length()-1;
                    state = EXPECTING_CDATA_CONTENT;
                    break;
                case EXPECTING_CDATA_CONTENT:
                    if (ch == ']') {
                        state = EXPECTING_CDATA_ENDIND_1;
                    } else {
                        // The state remains.
                    }
                    break;
                case EXPECTING_CDATA_ENDIND_1:
                    if (ch == ']') {
                        state = EXPECTING_CDATA_ENDIND_2;
                    } else {
                        state = EXPECTING_CDATA_CONTENT;
                    }
                    break;
                case EXPECTING_CDATA_ENDIND_2:
                    if (ch == '>') {
                        state = EXPECTING_TAG_CONTENT;
                    } else {
                        state = EXPECTING_CDATA_CONTENT;
                    }
                    break;
                case EXPECTING_TAG_END_NAME: 
                    string toMatch = stkActiveTags.top(); stkActiveTags.pop();
                    if (debug) printf("\tpopped activeTagName:%s\n", toMatch.c_str());
                    toMatch.push_back('>');
                    if (i+toMatch.length()-1 >= n || 0 != code.substr(i, toMatch.length()).compare(toMatch)) return false;
                    i += toMatch.length()-1;
                    state = (stkActiveTags.size() > 0 ? EXPECTING_TAG_CONTENT : STACK_PREEMPTIED);
                    break;
            }
            ++i;
        }
        
        return (state == STACK_PREEMPTIED);
    }
};
