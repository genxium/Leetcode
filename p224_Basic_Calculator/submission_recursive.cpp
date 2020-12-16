#define EXPRESSION 0
#define FACTOR 1
#define TERM 2

/**
 * The following rules are of left-associative evaluation.
 * 
 * exp := |term
 * 	|term [+ OR -] term 
 * 	|term [+ OR -] term [+ OR -] term ... 
 *
 * term := |factor
 * 	|factor [* OR /] factor  
 * 	|factor [* OR /] factor [* OR /] factor ...  
 *
 * factor := |number
 * 	|[+ OR -] number
 * 	|(exp)
 *	|[+ OR -] (exp)
 *
 * It's intended that regular-expression notation is NOT used to describe the rules.
 * */

void remove_spaces(char* s) {
        int effective_offset = 0;
        char* tmp_ptr = s;
        while (*tmp_ptr != '\n' && *tmp_ptr != '\0') {
                if (*tmp_ptr != ' ' && *tmp_ptr != '\t') {
                        *(s + effective_offset) = *tmp_ptr;    
                        ++effective_offset;
                }   
                ++tmp_ptr;
        }   
        while (*(s + effective_offset) != '\0') {
                *(s + effective_offset) = '\0'; 
                ++effective_offset;
        }
}

long read_int(char** p_s) {
        // (*p_s) should contain NO space 
        long ret = 0;
        while (isdigit(*(*p_s))) {
                ret *= 10; 
                ret += (long)(*(*p_s) - '0');
                ++(*p_s);
        }   
        return ret;
}

long left_associative_eval(char** p_s, int priority) {
	// (*p_s) should contain NO space
	long ret;
	long tmp;
	char binary_op = '\0';

	switch (priority) {
		case EXPRESSION: {
			// evaluate the first term
			ret = left_associative_eval(p_s, TERM);

			binary_op = *(*p_s);
				
			while (binary_op != '\0') {	
				switch (binary_op) {
					case '+': {
						++(*p_s);
						tmp = left_associative_eval(p_s, TERM);	
						ret += tmp;
						break;
					}
					case '-': {
						++(*p_s);
						tmp = left_associative_eval(p_s, TERM);	
						ret -= tmp;
						break;
					}
					default: {
						return ret;
					}
				}
				binary_op = *(*p_s);
			}

			break;
		}
		case TERM: {
			// evaluate the first factor 
			ret = left_associative_eval(p_s, FACTOR);

			binary_op = *(*p_s);

			while (binary_op != '\0') {	
				switch (binary_op) {
					case '*': {
						++(*p_s);
						tmp = left_associative_eval(p_s, FACTOR);
						ret *= tmp;
						break;
					}
					case '/': {
						++(*p_s);
						tmp = left_associative_eval(p_s, FACTOR);
						ret /= tmp;
						break;
					}
					default: {
						return ret;
					}
				}	
				binary_op = *(*p_s); 
			}

			break;	
		}
		case FACTOR: {
			int unary_factor = 1;
			char possible_unary_op = *(*p_s);
			if (possible_unary_op == '+' || possible_unary_op == '-') ++(*p_s);  
			if (possible_unary_op == '-') unary_factor = (-1); 
			if (isdigit(*(*p_s))) ret = read_int(p_s); 	
			else {
				++(*p_s);
				ret = left_associative_eval(p_s, EXPRESSION);
				++(*p_s);
			}
			ret *= unary_factor;
			break;
		}
	}
	return ret;
}

int calculate(char* s) {
    remove_spaces(s);
	long result = left_associative_eval(&s, EXPRESSION);
	return (int)result;
}
