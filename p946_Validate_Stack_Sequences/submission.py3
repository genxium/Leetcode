class Solution:
    def validateStackSequences(self, pushed: List[int], popped: List[int]) -> bool:
        if len(pushed) != len(popped):
            return False
        
        pushIdx, popIdx = 0, 0
        stk = []
        
        while (pushIdx < len(pushed) and popIdx < len(popped)):
            while ((len(stk) == 0 or stk[-1] != popped[popIdx]) and pushIdx < len(pushed)):
                stk.append(pushed[pushIdx])
                pushIdx += 1
            
            while (len(stk) > 0 and stk[-1] == popped[popIdx] and popIdx < len(popped)):
                stk.pop()
                popIdx += 1
                
        if pushIdx != popIdx:
            return False
        
        return True
