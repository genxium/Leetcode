/*
test cases
12
[10,8,0,5,3]
[2,4,1,1,3]
10
[0,4,2]
[2,1,3]
*/
class Car {
public:
    int pos, spd;
    Car(int aPos, int aSpd): pos(aPos), spd(aSpd) {}
};

class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = position.size();
        vector<double> firstCatchTime(n, -1.0);
        
        vector<Car> cars;
        for (int i = 0; i < n; ++i) cars.push_back(Car(position[i], speed[i]));
        sort(cars.begin(), cars.end(), [](Car const& lhs, Car const& rhs)  {
            return lhs.pos < rhs.pos;
        });
        
        stack<int> stk;
        for (int i = n-1; i >= 0; --i) {
            auto &thisCar = cars[i];
            while (!stk.empty()) {
                int repIdx = stk.top(); 
                auto &repCar = cars[repIdx];
                if (thisCar.spd <= repCar.spd) {
                    stk.pop();
                } else if (firstCatchTime[repIdx] > 0 && (double)(repCar.pos - thisCar.pos)/(thisCar.spd - repCar.spd) >= firstCatchTime[repIdx]) {
                    stk.pop();
                } else {
                    break;
                }
            }

            if (!stk.empty()) {
                // This is valid for "firstCatchTime[repIdx] == -1.0" too.
                int repIdx = stk.top();
                auto &repCar = cars[repIdx];
                firstCatchTime[i] = (double)(repCar.pos - thisCar.pos)/(thisCar.spd - repCar.spd);
            }

            stk.push(i);
        }
        
        int ans = 0;
        for (int i = n-1; i >= 0; --i) {
            if (firstCatchTime[i] == -1.0) {
                // WON'T ever join others
                ++ans;
                continue;
            } 
            if (cars[i].pos + firstCatchTime[i]*cars[i].spd > target) {
                // WON'T join others before "target"
                ++ans;
                continue;
            }
        }
        
        return ans;
    }
};
