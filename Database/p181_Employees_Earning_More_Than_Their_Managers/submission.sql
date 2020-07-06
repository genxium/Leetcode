# Write your MySQL query statement below
SELECT a.Name as Employee FROM Employee AS a JOIN Employee AS B ON a.ManagerId = b.Id AND a.Salary > b.Salary;
