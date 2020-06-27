class Solution {
public:
    
    string simplifyPath(string path) {
        int length = path.length();
        string ans = "/";
        stack<int> lastEffectiveSlashPosInAns;
        
        if (1 >= length) return ans;
        
        // skips the first '/', e.g. /home/test
        //                            ^
        int cur = 1; 
        while (cur < length) {
            int nextSlashPosInP = cur;
            while (path[nextSlashPosInP] != '/' && nextSlashPosInP < length) ++nextSlashPosInP;
            string content = path.substr(cur, nextSlashPosInP-cur);
            if (content.empty()) {
                // A double slash, do nothing.
            } else if (0 == content.compare(".")) {
                // Do nothing.
            } else if (0 == content.compare("..")) {
                // Snaps ans backward.
                if (lastEffectiveSlashPosInAns.empty()) {
                    // Do nothing.
                } else {
                    int tmp = lastEffectiveSlashPosInAns.top();
                    lastEffectiveSlashPosInAns.pop();
                    ans = ans.substr(0, tmp);
                }
            } else {
                // Normal content. Shall I process invalid content like "..."?
                lastEffectiveSlashPosInAns.push(ans.length());
                ans.append(content + "/");
            }
            cur = nextSlashPosInP + 1;
        }
        if (ans.length() > 1) {
            ans = ans.substr(0, ans.length() - 1);            
        }
        return ans;
    }
};
