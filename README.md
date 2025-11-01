# A SIMPLE DATABASE SCHEMA IN C LANGUAGE 
---
for educational purposes.
a very basic database implemented in C , with the following features :
1/ Add a user
-> insert a user record (username,email,age,...)
2/ Validate user input
-> prevent invalid entries
3/ Search for user by key
-> hash-based lookup
4/ Export to JSON
-> save the current database to JSON file
5/ Others
-> List all users
-> List single user
-> Update user

TODO:
- make the database asynchronous
- delete some users
- adding a reference

build and run 
```
gcc -o test database.c 
./test
```
Example Ouput :
view the file db.json
```
[
  {
    "username": "moezgwall",
    "email": "moez@Iosekva.com",
    "age": 26
  },
  {
    "username": "zozssin",
    "email": "unclebob@solid.com",
    "age": 99
  }
]
```

