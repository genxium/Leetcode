# Write your MySQL query statement below
SELECT a.score AS `score`, COUNT(DISTINCT b.score)+1 AS `rank` 
FROM Scores a LEFT JOIN Scores b /* LEFT JOIN to preserve rows of "a" where no "a.score < b.score" exists for that "a.score" */ 
ON a.score < b.score
GROUP BY a.Id
ORDER BY a.score DESC;
