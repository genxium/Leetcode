class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        vector<int> remaining;
        list<int> buff;
        
        for (auto asteroid : asteroids) {
            if (asteroid > 0) buff.push_back(asteroid);
            else {
                while (!buff.empty() && buff.back() < abs(asteroid)) {
                    buff.pop_back();
                }
                if (!buff.empty() && buff.back() == abs(asteroid)) {
                    buff.pop_back(); 
                    continue;
                } else if (buff.empty()) {
                    remaining.push_back(asteroid);
                } else {
                    // Do nothing
                }
            }
        }
        
        while (!buff.empty()) {
            remaining.push_back(buff.front());
            buff.pop_front();
        }
        
        return remaining;
    }
};
