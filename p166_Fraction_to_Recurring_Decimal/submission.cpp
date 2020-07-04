class Solution {
public:    
    int signOf(int &numerator, int& denominator) {
        if (0 == numerator) return 0;
        if (0 < numerator) {
            if (0 < denominator) return +1;
            else return -1;
        } else {
            if (0 < denominator) return -1;
            else return +1;
        }
    }
    
    uint absu(int v) {
        if (v == INT_MIN) {
            return (uint)INT_MAX + 1u;
        }
        return (uint)abs(v);
    }
    
    string fractionToDecimal(int numerator, int denominator) {
        /*
        test case #1
        -1
        -2147483648

        test case #2
        4
        333
        
        test case #3
        -1000
        103
        */
        string toRet = "";

        unordered_map<uint, int> remainderPos;
        int sign = signOf(numerator, denominator);

        uint un = absu(numerator);
        uint ud = absu(denominator);
        
        uint quotient = (un/ud);
        toRet.append(to_string(quotient));
        
        bool dotted = false;
        
        uint remainder = (un%ud), recursAtPos = -1;
        int prevSubquotient = -1;
        //printf("un == %u, ud == %u, checking remainder == %u\n", un, ud, remainder);

        while (0u < remainder) {
            if (!dotted) {
                toRet.push_back('.');
                dotted = true;
            }
            
            if (-1 != prevSubquotient) {    
                toRet.push_back('0'+prevSubquotient);
                prevSubquotient = -1; // Important for making the "recurring segment location" correct.
            }
            
            //printf("toRet:%s, checking remainder == %u\n", toRet.c_str(), remainder);
            if (remainderPos.find(remainder) == remainderPos.end()) {
                remainderPos[remainder] = toRet.length();
                prevSubquotient = 0;
                uint tmpR = remainder;
                // Consider the extreme case where "ud == -INT_MIN". The following snippet is equivalent to "remainder = (remainder*10)/ud" for overflowing numbers, whilst leaving "prevSubquotient = (remainder*10)%ud" for the next cycle to handle.
                int count = 1;
                while (count < 10) {
                    remainder += tmpR;
                    if (remainder >= ud) {
                        remainder -= ud;
                        ++prevSubquotient;
                    }
                    ++count;
                }
            } else {
                recursAtPos = remainderPos[remainder];
                break;
            }
        }
        // wrap up
        if (-1 != prevSubquotient) {   
            //printf("wrapping up prevSubquotient == %u\n", prevSubquotient);
            toRet.push_back('0'+prevSubquotient);
        }
        if (-1 != recursAtPos) {
            int recurringLength = toRet.length() - recursAtPos;
            string recurringPart = toRet.substr(recursAtPos, recurringLength);
            string firstPart = toRet.substr(0, recursAtPos);
            toRet = firstPart + "(" + recurringPart + ")";
        }
        if (sign == -1) {
            toRet = "-" + toRet;
        }
        return toRet;
    }
};
