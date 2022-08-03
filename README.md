# MyDB
A Transaction level Concurrent SQL Server

- SQL Server like database, which supports **concurrent** Addition, Deletion, Modification and Query operation.
- Support **keep-alive** TCP connection with **Multi-Thread** or high concurrency connection with **Thread-Pool**.
- Use **2-phase commit** and **Index S/X lock** to support concurrency while detect **deadlock** to **rollback** with log.
- Designed **MySIAM** liked disk **linked** Storage System with in-memory **B-Plus Tre**e to Optimize Query.
- Designed **Parser** and **Interpreter**, responsible for **Syntactic Semantic Error** and generate Query Objects.
- Support Create, Insert, Delete, Drop, Select, Limit, Cross-join, Alter, Sort, Group by, etc.
- Use **LRU-Cache** to improve system speed.


# Code Structure Of SQL Server

```
── CMakeLists.txt
├── CMakeSettings.json
├── makefile
├── scripts
│   ├── final.txt
│   ├── test1.txt
│   └── test2.txt
├── src
│   ├── CMakeLists.txt
│   ├── Controller
│   │   ├── Application.cpp
│   │   ├── Application.hpp
│   │   ├── CommandProcessors
│   │   ├── Locker
│   │   ├── Managers
│   │   └── ParserHelpers
│   ├── DefaultTests
│   │   ├── Faked.cpp
│   │   ├── Faked.hpp
│   │   ├── ScriptRunner.hpp
│   │   ├── TestAutomatic.hpp
│   │   ├── TestManually.hpp
│   │   └── TestSequencer.hpp
│   ├── Model
│   │   ├── Alter.hpp
│   │   ├── Attribute.cpp
│   │   ├── Attribute.hpp
│   │   ├── Database.cpp
│   │   ├── Database.hpp
│   │   ├── Entity.cpp
│   │   ├── Entity.hpp
│   │   ├── Log.cpp
│   │   ├── Log.hpp
│   │   ├── Row.cpp
│   │   ├── Row.hpp
│   │   ├── SQLQuery.cpp
│   │   ├── SQLQuery.hpp
│   │   ├── Statements
│   │   └── Storage
│   ├── Server
│   │   ├── SQLServer.cpp
│   │   ├── SQLServer.hpp
│   │   └── Socket
│   ├── Util
│   │   ├── BasicTypes.hpp
│   │   ├── Config.hpp
│   │   ├── Errors.hpp
│   │   ├── FolderReader.hpp
│   │   ├── FormatString.hpp
│   │   ├── Helpers.hpp
│   │   ├── Timer.hpp
│   │   └── keywords.hpp
│   ├── View
│   │   ├── FolderView.hpp
│   │   ├── QueryResultView.hpp
│   │   ├── TableView.cpp
│   │   ├── TableView.hpp
│   │   ├── TimerView.hpp
│   │   └── View.hpp
│   └── main.cpp
├── tests
│   ├── AutograderTests.cpp
│   ├── CMakeLists.txt
│   ├── CustomTests.cpp
│   ├── SanityCheck.cpp
│   └── main.cpp
└── tools
    ├── RunLinter.py
    └── build.sh


```


# Code Structure Of SQL Client


```

├── CMakeLists.txt
├── PracticalSocket.cpp
├── PracticalSocket.hpp
├── client
├── client.cpp
├── makefile
└── tools
    └── build.sh
    
```

# Architecture Overview

<img width="1154" alt="image" src="https://user-images.githubusercontent.com/63135646/182532962-d10673c2-b574-46d3-9a0b-d2408da39c4f.png" style="zoom:50%">



# Supported Operation



## Supported Statements

### Application Statement

```mysql
version; # Version 1.0
help; # Help system available
quit; # DB::141 is shutting down
```

### DBStatement

#### Create database statement

```mysql
CREATE DATABASE testing1;
Query OK, 1 row affected (0.01 secs)  
```

#### Drop database statement

```mysql
DROP DATABASE {db-name}
Query OK, 0 rows affected (0.00)  
```

#### Show databases statement

```mysql
> show databases;
+--------------------+
| Database           |
+--------------------+
| foo                |
| bar                |
| wiltz              |
+--------------------+
3 rows in set (0.02 sec)
```

#### Use database statement

```mysql
> use testing;
Database changed
```

#### Dump database statement

```mysql
> dump database testdb;
+----------------+--------+---------------+
| Type           | Id     | Extra         |
+----------------+--------+---------------+
| Meta           | 0      |               |
-----------------+--------+---------------+
| Entity         | 1      | Users         |
-----------------+--------+---------------+
| Data           | 1      | Users         |
-----------------+--------+---------------+
| Free           | 0      |               |
-----------------+--------+---------------+
| Data           | 3      | Users         |
-----------------+--------+---------------+
5 rows in set (0.02 sec)
```

### SQLStatement

#### Create table statement

```mysql
> CREATE TABLE tasks (
  	id INT AUTO_INCREMENT PRIMARY KEY,
  	title VARCHAR(100) NOT NULL,
  	price FLOAT DEFAULT 0.0,
  	status BOOLEAN DEFAULT FALSE,
	)
Query OK, 1 row affected (0.002 sec)
```

#### Show tables statement

```mysql
> show tables;
+----------------------+
| Tables_in_mydb       |
+----------------------+
| groups               |
| users                |
+----------------------+
2 rows in set (0.000025 sec.)
```

#### Drop table statement

```mysql
> drop table groups;
Query OK, 0 rows affected (0.02 sec)
```

#### Describe table statement

```mysql
> DESCRIBE tasks;
+-----------+--------------+------+-----+---------+-----------------------------+
| Field     | Type         | Null | Key | Default | Extra                       |
+-----------+--------------+------+-----+---------+-----------------------------+
| id        | integer      | NO   | YES | NULL    | auto_increment primary key  |
| title     | varchar(100) | NO   |     | NULL    |                             |
| price     | float        | YES  |     | 0.0     |                             |
| due_date  | date         | YES  |     | NULL    |                             |
| status    | boolean      | YES  |     | FALSE   |                             |
+-----------+--------------+------+-----+---------+-----------------------------+
5 rows in set (0.000043 sec.)

```

#### Insert statement

```mysql
INSERT INTO users 
  ('first_name', 'last_name', 'email') 
VALUES 
  ('David','He', 'dyhe@ucsd.edu'),
  ('Sai', 'Komatineni', 'saikomatineni@engucsd.edu');
```

#### Select statement

```mysql
> SELECT * from Users;
+--------------------+--------------+
| id  | first_name   | last_name    |
+-----+--------------+--------------+
| 1   | david        | he           |
| 3   | rick         | gessner      |
+-----+--------------+--------------+
3 rows in set (0.00231 sec)
```

```mysql
> SELECT id, first_name, last_name, zipcode from Users where zipcode>92120 order by zipcode LIMIT 2
+--------------------+--------------+--------------+
| id  | first_name   | last_name    | Zipcode      |
+-----+--------------+--------------+--------------+
| 3   | Anirudh      | Swaninthan   | 92126        |
| 1   | Pu           | Cheng        | 92127        |
+-----+--------------+--------------+--------------+
2 rows in set (0.00123 sec)
```

```mysql
> select last_name, title from Authors left join Books on Authors.id=Books.author_id;
+-----------+-------------------------------------------+
| last_name | title                                     |
+-----------+-------------------------------------------+
| Rowling   | Harry Potter and the Sorcerer's Stone     |
| Rowling   | Harry Potter and the Philosopher's Stone  |
| Rowling   | Harry Potter and the Prisoner of Azkaban  |
| Rowling   | Harry Potter and the Chamber of Secrets   |
| Rowling   | Harry Potter and the Goblet of Fire       |
| Rowling   | Harry Potter and the Order of the Phoenix |
| Rowling   | Harry Potter and the Half-Blood Prince    |
| King      | Carrie                                    |
| King      | The Dark Tower                            |
| King      | The Green Mile                            |
| Nguyen    | NULL                                      |
+-----------+-------------------------------------------+
11 rows in set (0.00 sec)
```

#### Update statement

```mysql
UPDATE Users SET "zipcode" = 92127 WHERE zipcode>92100;
Query Ok. 2 rows affected (0.000087 sec)
```

#### Delete statement

```mysql
DELETE from Users where zipcode>92124;
Query Ok. 1 rows affected (0.000023 sec)
```

#### Alter statement

```mysql
ALTER TABLE Books add pub_year varchar(4);  
Query Ok, 20 rows affected (0.00123 secs)
```

​    

# How to Run The Code

Compile SQL server:

```console

$ make

```

Run SQL server:

```console

$ ./SQLserver  <Server Address> <Server Port> <Connection Mechenism> 

$ ./SQLserver 127.0.0.1 8080 long 

$ ./SQLserver 127.0.0.1 8080 short

```



Compile SQL Client:

```console

$ make

```

Run SQL client:

```console

$ ./client  <Server Address> <Server Port> <Connection Mechenism> 

$ ./client 127.0.0.1 8080 long 

$ ./client 127.0.0.1 8080 short

```



# Environment

- C++ 17
- MacOS



# Test

- provide some integrated test 
- also prove sript runner to do concurrent test with docker

