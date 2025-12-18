SELECT name_dish FROM dishes
WHERE category_id = 2;

SELECT * FROM orders
WHERE order_date > '2023-06-12' AND order_date < '2023-06-16';

SELECT SUM(dishes.price * orders.quantity)
FROM orders
JOIN dishes ON orders.dish_id = dishes.id
WHERE order_date = '2023-06-16';

SELECT 
dishes.name_dish
FROM orders
JOIN dishes ON orders.dish_id = dishes.id
GROUP BY dishes.id, dishes.name_dish
HAVING COUNT(orders.dish_id) = (
    SELECT MAX(dish_count)
    FROM (
        SELECT COUNT(dish_id) as dish_count
        FROM orders
        GROUP BY dish_id
    )
);

SELECT 
categories.category_name,
SUM(orders.quantity * dishes.price) AS total_revenue
FROM orders 
JOIN dishes ON orders.dish_id = dishes.id
JOIN categories ON dishes.category_id = categories.id
WHERE order_date > '2023-06-01' AND order_date < '2023-07-01'
GROUP BY categories.id, categories.category_name

UPDATE dishes
SET price = price*1.1
WHERE category_id = 5;

BEGIN TRANSACTION;
	DELETE FROM dishes WHERE name_dish = 'Жареная треска';
	DELETE FROM orders WHERE dish_id = 6;
COMMIT;

