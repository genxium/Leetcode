# Write your MySQL query statement below
SELECT a.Name AS Customers FROM Customers AS a WHERE (SELECT COUNT(b.Id) FROM Orders AS b WHERE b.CustomerId = a.Id) = 0;
