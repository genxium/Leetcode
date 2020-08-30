/**
 * Your NestedIterator object will be instantiated and called as such:
 * NestedIterator i(nestedList);
 * while (i.hasNext()) cout << i.next();
 */

/**
 * // This is the interface that allows for creating nested lists.
 * // You should not implement it, or speculate about its implementation
 * class NestedInteger {
 *   public:
 *     // Return true if this NestedInteger holds a single integer, rather than a nested list.
 *     bool isInteger() const;
 *
 *     // Return the single integer that this NestedInteger holds, if it holds a single integer
 *     // The result is undefined if this NestedInteger holds a nested list
 *     int getInteger() const;
 *
 *     // Return the nested list that this NestedInteger holds, if it holds a nested list
 *     // The result is undefined if this NestedInteger holds a single integer
 *     const vector<NestedInteger> &getList() const;
 * };
 */

/*
test case #1
[]

test case #2
[[1]]

test case #3
[[[1]]]

test case #4
[1,[4,[],[],[5],[[6]]]]
*/

#define BEGIN_ITR_TYPE vector<NestedInteger>::iterator 
#define END_ITR_TYPE vector<NestedInteger>::iterator 
#define CURSOR_ITR_TYPE vector<NestedInteger>::iterator 

class NestedIterator {
private: 
    vector<NestedInteger> theList;
    vector<NestedInteger>::iterator cur;

    stack< pair<CURSOR_ITR_TYPE, END_ITR_TYPE> > stk;

    void runOutOfStackedEnd() {
        while (!stk.empty() && cur == stk.top().second) {
            cur = stk.top().first; // for each element on stack, "first" is 1-layer above "second"
            stk.pop();
            ++cur;
        } 
        // now that "cur == theList.end()" or "cur points to an non-empty element" 
    }

    void diveIntoNextInt() {
        //printf("diveIntoNextInt #1\n");
        runOutOfStackedEnd();
        //printf("diveIntoNextInt #2\n");
        while (cur != theList.end() && !cur->isInteger()) {
            auto flattened = &(cur->getList()); // 1-layer down
            if (flattened->empty()) {
                //printf("diveIntoNextInt #3, got a new empty cur->getList()\n");
                ++cur; 
                /*
                Consider an edge case as follows
                ```
                [..., [42, 43, 21, ..., 33, []], [56], 7]
                                            ^
                                            cur
                      ^
                stk.top().first
                ```
                , where we just came to an empty list from "33", yet the empty list is also the end of the wrapping list.
                */        
                runOutOfStackedEnd();
                continue;
            }
            auto prev = cur;
            stk.push({prev, flattened->end()}); // note that "prev" is 1-layer above "flattened.end()"
            cur = flattened->begin();
            //printf("diveIntoNextInt #4, pushed 1-layer\n");
        }
        // now that "cur == theList.end()" or "cur->isInteger()" 
        //printf("diveIntoNextInt, returning\n");
    }

public:
    NestedIterator(vector<NestedInteger> &nestedList) {
        theList = nestedList;
        cur = theList.begin();

        diveIntoNextInt();
    }

    int next() {
        int toRet = cur->getInteger();  
        ++cur;
        diveIntoNextInt();
        return toRet;
    }

    bool hasNext() {
        return cur != theList.end();
    }
};

/**
 * Your NestedIterator object will be instantiated and called as such:
 * NestedIterator i(nestedList);
 * while (i.hasNext()) cout << i.next();
 */

