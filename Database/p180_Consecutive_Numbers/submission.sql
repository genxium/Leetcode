# Write your MySQL query statement below
SELECT temp.curNum AS ConsecutiveNums
FROM
(
  SELECT CASE WHEN @prevNum = a.Num THEN @sameAsPrevCount := @sameAsPrevCount + 1 ELSE @sameAsPrevCount := 1 END AS consecCount, @prevNum := a.Num As curNum
  FROM Logs AS a, (SELECT @prevNum := -2147483648) AS init, (SELECT @sameAsPrevCount := 1) AS init3
) AS temp
WHERE temp.consecCount >= 3
GROUP BY ConsecutiveNums
