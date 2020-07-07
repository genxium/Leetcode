# Write your MySQL query statement below
DELETE a FROM Person AS a, Person AS b WHERE a.Email = b.Email AND a.Id > b.Id;
