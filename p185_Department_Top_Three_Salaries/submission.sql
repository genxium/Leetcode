# Write your MySQL query statement below
SELECT temp2.Department, temp2.Employee, temp2.Salary
FROM
(
  SELECT 
  (CASE WHEN temp.deptId <> @prevDeptId THEN @salaryInDeptCount := 1 ELSE (CASE WHEN temp.Salary <> @prevSalaryInDept THEN @salaryInDeptCount := @salaryInDeptCount + 1 ELSE @salaryInDeptCount END) END) AS SalaryInDeptCount, 
  temp.Department, 
  temp.Employee, 
  temp.Salary,
  @prevSalaryInDept := temp.Salary AS prevSalaryInDept,
  @prevDeptId := temp.deptId AS prevDeptId
  FROM
  (SELECT b.Id AS deptId, b.Name AS Department, a.Name AS Employee, a.Salary AS Salary FROM Employee AS a JOIN Department AS b ON a.DepartmentId = b.Id ORDER BY deptId, Salary DESC) AS temp, (SELECT @prevDeptId := -1, @prevSalaryInDept := 0, @salaryInDeptCount := 1) AS init
) AS temp2
WHERE temp2.SalaryInDeptCount <= 3
ORDER BY Department
