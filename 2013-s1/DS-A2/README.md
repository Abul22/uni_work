This project is a very simple database management system implement by myself.

###Usage
```
make
```

###Task 1
Load a databese relation writing a heap file

```
./dbload [-p pagesize] [-d directory] guilds
./dbload [-p pagesize] [-d directory] characters

```

###Task 2
Selecet from realtion using heap file

```
./dbquery [-p pagesize] [-d directory] select guilds attribute value
```

###Task 3
Nested loop join

```
./dbquery [-p pagesize] [-d directory] [-b buffer] [-t] outer_relation join inner_relation
```

###Task 4
Nested loop join Experiments - See report.txt

###Task 5
Full query plans

```
./dbquery [-p pagesize] [-d directory] [-b buffer] [-t] outer_relation join inner_relation select attribute value project attribute
```
For use detail see report.txt

