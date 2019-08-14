# Database Generator
- Used to generate databases to run the system with
- The program reads names.txt to generate the names, and will generate the
  specified data for all the names in that list.
- When generating databases, you will need to specifiy how many days of data to
  be generated and how large each individual piece of data is.

 
## How to generate a database
- Make sure names.txt exists and is populated ( gen-names.sh can help here )
- Build and run the program:
```
./build.sh -r       ( remove any previous database here )
./dbstarter --user  ( generate the user accounts )
./dbstarter --data { # of days } { size of data in bytes }
```
- Now copy the database to whatever directory it will need to be run from.
- If running locally, put the database file in the root of SAMPL/


