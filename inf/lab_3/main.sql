CREATE DATABASE restaurant_menu;

CREATE TABLE dishes(
  id INT PRIMARY KEY,
  name_dish VARCHAR(100),
  category_id INT,
  price FLOAT
);

CREATE TABLE categories(
  id int PRIMARY KEY,
  category_name VARCHAR(100)
);

CREATE TABLE orders(
  order_id INT,
  dish_id INT,
  order_date DATE,
  quantity INT
);

ALTER TABLE dishes
ADD CONSTRAINT fk_dishes_category
FOREIGN KEY (category_id) REFERENCES categories(id)
ON DELETE SET NULL;

ALTER TABLE orders
ADD CONSTRAINT fk_orders_dishes
FOREIGN KEY (dish_id) REFERENCES dishes(id)
ON DELETE SET NULL;

INSERT INTO categories(id, category_name)
VALUES
  (1, 'Супы'),
  (2, 'Основные блюда'),
  (3, 'Закуски'),
  (4, 'Салаты'),
  (5, 'Десерты'),
  (6, 'Напитки')

INSERT INTO dishes(id, name_dish, category_id, price)
VALUES
  (1, 'Борщ', 1, 200),
  (2, 'Рассольник', 1, 150),
  (3, 'Суп гороховый', 1, 180),
  (4, 'Гречка с котлетами', 2, 250),
  (5, 'Паста карбонара', 2, 300),
  (6, 'Жареная треска', 2, 270),
  (7, 'Картошка фри', 3, 130),
  (8, 'Сырные палочки', 3, 150),
  (9, 'Лаваш', 3, 100),
  (10, 'Цезарь', 4, 210),
  (11, 'Салат витаминный', 4, 200),
  (12, 'Чизкейк', 5, 280),
  (13, 'Тирамису', 5, 250),
  (14, 'Сок яблочный', 6, 120),
  (15, 'Вода негазированная', 6, 80),
  (16, 'Чай чёрный', 6, 250)

INSERT INTO orders(order_id, dish_id, order_date, quantity)
VALUES
  (1, 1, '2023-06-13', 1),
  (1, 5, '2023-06-13', 1),
  (1, 16, '2023-06-13', 2),
  (2, 4, '2023-06-15', 1),
  (2, 10, '2023-06-15', 2),
  (2, 14, '2023-06-15', 1),
  (3, 6, '2023-06-16', 1),
  (3, 7, '2023-06-16', 2),
  (3, 16  , '2023-06-16', 1)