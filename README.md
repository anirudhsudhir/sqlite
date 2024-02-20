# sqlite

A partial clone of SQLite written in C.

It supports addition of entries in the form of (user id, username, email).

Data is flushed to the disk on exiting from the db.

## Usage

1. Clone the repository and build the application

```bash
cd sqlite/src/
make
```

2. Run the application by passing in a " .db " file as the argument. If the file isn't present, it will be created once the user exits the db

```bash
./db new.db
```

Built while following [this](https://cstack.github.io/db_tutorial/) tutorial.
