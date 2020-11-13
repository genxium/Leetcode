class Solution {
private:
    bool isValidIPv4Seg(string &seg) {
        int val = 0;
        for (int i = 0; i < seg.length(); ++i) {
            if (!isdigit(seg[i])) return false;
            val *= 10;
            if (val >= 256) return false;
            val += seg[i]-'0';
        }
        if (val == 0) {
            return seg.length() == 1 && seg[0] == '0';
        } 
        return seg[0] != '0' && val < 256;
    }
    
    bool isValidIPv4(string &IP) {
        if (!isdigit(IP.back())) return false;
        istringstream ss(IP);
        string token;
        
        int count = 0;
        while(getline(ss, token, '.')) {
            // printf("isValidIPv4, token == %s\n", token.c_str());
            if (!isValidIPv4Seg(token)) return false;
            ++count; 
            if (count > 4) return false;
        }
                
        return count == 4;
    }
public:
    string validIPAddress(string IP) {
        regex ipv4Rex("^^(?:(?:25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\.){3}(?:25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])$", regex_constants::ECMAScript | regex_constants::icase);
        if (regex_search(IP, ipv4Rex)) {
            return "IPv4";
        }
        
        // if (isValidIPv4(IP)) {
        //     return "IPv4";
        // }
        
        regex ipv6Rex("^(([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4})$", regex_constants::ECMAScript | regex_constants::icase);
        if (regex_search(IP, ipv6Rex)) {
            return "IPv6";
        }
        
        return "Neither";
    }
};
