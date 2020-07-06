# Write your MySQL query statement below
SELECT temp.Email FROM 
(SELECT Id, Email, COUNT(Email) AS c FROM Person GROUP BY Email) AS temp
WHERE temp.c >= 2
