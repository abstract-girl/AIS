use archdb; 
show tables;

CREATE TABLE IF NOT EXISTS Item (
  id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(255) NOT NULL,
  category VARCHAR(255) NOT NULL,
  availability BOOLEAN NOT NULL,
  description TEXT,
  price DECIMAL(10, 2) NOT NULL
);

CREATE TABLE IF NOT EXISTS User (
  id VARCHAR(255) PRIMARY KEY,
  login VARCHAR(255) NOT NULL,
  first_name VARCHAR(255) NOT NULL,
  last_name VARCHAR(255) NOT NULL,
  email VARCHAR(255) NOT NULL,
  title VARCHAR(255),
  password VARCHAR(255),
  role VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS Bucket (
  id INT AUTO_INCREMENT PRIMARY KEY,
  user_id VARCHAR(255),
  FOREIGN KEY (user_id) REFERENCES User(id)
);

CREATE TABLE IF NOT EXISTS Bucket_Item (
  bucket_id INT,
  item_id INT,
  PRIMARY KEY (bucket_id, item_id),
  FOREIGN KEY (bucket_id) REFERENCES Bucket(id),
  FOREIGN KEY (item_id) REFERENCES Item(id)
);

INSERT INTO Item (name, category, availability, description, price)
VALUES ('Milk', 'Dairy', true, 'Best milk in the country!', 19.99),
       ('Eggs', 'Dairy', false, 'Fresh from under the chicken!', 29.99);

INSERT INTO User (id, login, first_name, last_name, email, title, role, password)
VALUES ('RANDMON-ID', 'ivan-sidorov', 'Ivan', 'Sidorov', 'isidorov@ya.ru', 'Mr.', 'customer', 'fsklmfkms'),
       ('ANOTHER-RANDOM-ID', 'val-petrov', 'Valery', 'Petrov', 'Val.petrov@ya.ru', 'Ms.', 'customer', 'sdflmfemfk');

INSERT INTO Bucket (user_id)
VALUES ('RANDMON-ID'),
       ('ANOTHER-RANDOM-ID');

INSERT INTO Bucket_Item (bucket_id, item_id)
VALUES (1, 1),
       (1, 2);

