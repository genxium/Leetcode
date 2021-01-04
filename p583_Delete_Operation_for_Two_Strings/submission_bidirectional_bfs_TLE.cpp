bool const debug = false;

/*
test cases
"sea"
"eat"
"jaojiajidfjaioijfd"
"ajsaidufojajafsjf"
""
"a"
*/
class Solution {
public:
    int minDistance(string word1, string word2) {
        if (word1.compare(word2) == 0) return 0;
        
        unordered_map<string, int> forwardDis;
        unordered_map<string, int> reverseDis;
        
        queue<string> q, rq;
        q.push(word1);
        rq.push(word2);

        forwardDis[word1] = 0;
        reverseDis[word2] = 0;
        
        int forwardLv = 0, reverseLv = 0;
        
        while (!q.empty() || !rq.empty()) {
            int sz = q.size();
            if (sz > 0) {
                ++forwardLv;
                for (int i = 0; i < sz; ++i) {
                    auto fs = q.front(); q.pop();
                    for (int j = 0; j < fs.length(); ++j) {
                        string clone(fs);
                        clone.erase(clone.begin() + j);
                        if (forwardDis.find(clone) != forwardDis.end()) {
                            continue;
                        } else if (reverseDis.find(clone) != reverseDis.end()) {
                            if (debug) printf("%s is found in reverseDis, returning forwardLv:%d, reverseLv:%d\n", clone.c_str(), forwardLv, reverseLv);
                            return forwardLv+reverseDis[clone];
                        } else {
                            forwardDis[clone] = forwardLv;
                            q.push(clone);
                        }
                    }
                }
            }
            
            int rsz = rq.size();
            if (rsz > 0) {
                ++reverseLv;
                for (int i = 0; i < rsz; ++i) {
                    auto fs = rq.front(); rq.pop();
                    for (int j = 0; j < fs.length(); ++j) {
                        string clone(fs);
                        clone.erase(clone.begin() + j);
                        if (reverseDis.find(clone) != reverseDis.end()) {
                            continue;
                        } else if (forwardDis.find(clone) != forwardDis.end()) {
                            if (debug) printf("%s is found in forwardDis, returning forwardLv:%d, reverseLv:%d\n", clone.c_str(), forwardLv, reverseLv);
                            return forwardDis[clone]+reverseLv;
                        } else {
                            reverseDis[clone] = reverseLv;
                            rq.push(clone);
                        }
                    }
                }
            }
        }
        
        return forwardLv+reverseLv; // Can always come down to an answer after deleting all.
    }
};
