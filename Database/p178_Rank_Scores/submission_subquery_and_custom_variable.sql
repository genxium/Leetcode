# Write your MySQL query statement below
SELECT temp.curScore AS Score, CAST((CASE WHEN temp.curScore < temp.prevScore THEN @rank := @rank+1 ELSE @rank END) AS UNSIGNED) AS `Rank` 
FROM 
(
  SELECT @prevScore AS prevScore, @prevScore := a.Score As curScore
  FROM Scores AS a, (SELECT @prevScore := MAX(Score) FROM Scores) AS init
  ORDER BY a.Score DESC
) AS temp, (SELECT @rank := 1) AS init2

