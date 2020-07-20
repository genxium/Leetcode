# Write your MySQL query statement below

SELECT 
Request_at AS Day,
ROUND(SUM(CASE WHEN `Status` <> 'completed' THEN 1 ELSE 0 END)/COUNT(*), 2) AS "Cancellation Rate"
FROM
(SELECT t.Id, t.Status, t.Request_at FROM Trips as t JOIN Users as u JOIN Users as v ON 
  (t.Client_Id = u.Users_Id AND u.Role = "client" AND u.Banned = "No")
  AND
  (t.Driver_Id = v.Users_Id AND v.Role = "driver" AND v.Banned = "No")
  AND
  t.Request_at BETWEEN "2013-10-01" AND "2013-10-03"
  GROUP BY Id
) as tmp
GROUP BY Day;
