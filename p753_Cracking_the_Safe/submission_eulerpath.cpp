/*
test cases
1
1
1
2
2
2
4
4
4
10
*/
class Solution {
public:
    void eulerPath(string &wordPrefixNode, int k, unordered_set<string> &visited, string &ans) {
        for (int d = 0; d < k; ++d) {
            string newWord(wordPrefixNode); newWord.push_back('0' + d);
            if (visited.count(newWord)) continue;
            visited.insert(newWord);
            /*
            [WARNING]
            The ORDER of the following 2 statements is critical!
            */
            string nextWordPrefixNode = newWord.substr(1);
            eulerPath(nextWordPrefixNode, k, visited, ans); 
            ans.push_back('0' + d);
        }
    }
    
    string crackSafe(int n, int k) {
        /*
        Denote that a "WordGraph = {WordNode, OutEdge}", where the "WordNode"s are ALL different n-length words consisting of [0, ..., k-1], and an "OutEdge" of a "WordNode" appends a digit to the current word and removes the front digit, e.g. "0100" -2-> "1002" with '2' being the "OutEdge of 0100" in this example. 
        
        The problem asks to minimize the total cost of visiting all of the "WordNode"s, where it's obvious that each "cost(OutEdge) = 1". Here are a few noticeable characteristics of the "WordGraph". 
        - (a.) Multiple "OutEdge"s could converge to a same "WordNode", e.g. "*100" -2-> "1002".
        - (b.) Some "OutEdge"s could form an immediate ring iff the current word consists of "all identical digits", e.g. "1111" -1-> "1111". 
        - (c.) EVERY "WordNode" has a "predecessor".
        
        The point (c.) is CRITICAL, because if there were a unique choice of "EntryWordNode: v" to get the "MinCost", we could have started from the "predecessor of v: u" and get the exact same "MinCost", i.e. added 1 cost between "u -> v" but reduced 1 cost to reach "u". 
        
        [LEMMA#1] Therefore, we can start from any "WordNode" and get the "MinCost" eventually.
        
        However, it's still difficult to solve the problem in the "WordGraph", because we couldn't efficiently reuse the "prefix", i.e. for "WordNode: u = 1100" and 3 different "OutEdge"s, 
        - "1100" -0-> "v0 = 1000"
        - "1100" -1-> "v1 = 1001"
        - "1100" -2-> "v2 = 1002"
        , it's WRONG to say that "v0, v1, v2 can be produced by sliding through 1100012" -- regardless of using DFS or BFS, the dilema sits at the nature of the "WordGraph". 
        */
        
        /*
        Assume that we pick each "WordNode.substr(0, length-1)" to form a collection of "WordPrefixNode"s (merge if having same content), then remove the original "WordGraph", and add "OutEdge"s of the same definition to the new graph, namely "WordPrefixGraph = {WordPrefixNode, OutEdge}". 
        
        [LEMMA#2] In this new "WordPrefixGraph", we only have to traverse all "OutEdge"s to get the "MinCost" of producing all the "n-length Words" -- which is equivalent to an EulerPath problem.
        */
        
        if (1 == n && 1 == k) return "0";
        unordered_set<string> visited;
        string ans = "";
        
        // [LEMMA#1]
        string startWordPrefixNode = ""; 
        for (int i = 0; i < n-1; ++i) {
            startWordPrefixNode.push_back('0');
        }
        
        // [LEMMA#2]
        eulerPath(startWordPrefixNode, k, visited, ans);
        ans.append(startWordPrefixNode);
        
        return ans;
    }
};
