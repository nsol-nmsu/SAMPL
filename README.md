# SAMPL

- Implementation of SAMPL
- Each entity in the network has their own working directory.

## Quick Start

### Set up the environment and build the source codes

- In the root

```
mkdir obj
cd lib && make
cd ..
./rebuild.sh
```

- A Small sample database has been provided, as well as a short list of random
  names to build a new database with different parameters.
- The sample database includes 8 users, with 5 days worth of data. There are 32
  entries of data, creating one merkle tree , and each data size is 10 bytes, to
  keep the size of the database file small.
```
cd dbstarter
./build.sh
{ optional: alter names.txt }
./p --user
./p --data { # now of days of survaillance } { Size of each data entry }

```
- copy the generated database to the base directory to use
- **Make sure the SR__names.txt file the Law entity reads is the same as
  names.txt that was used to generate the database. If nothing was touched, it
  should be set**

- Now run test scenarion on the local machine
```
./val-run.sh { # of people under survaillance } { # of days of survaillance } {
offset of people to run same tests with different subset of people }
```
- The Enforcer entity logs show the result of the zero knowledge proof
  verification.

