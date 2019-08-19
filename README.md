# SAMPL

- Implementation of SAMPL
- Each entity in the network has their own working directory.
- Within each directory of the Entitiy is their own main.c that contains their
  logic. There is a Makefile to compile them.


## Quick Start

### Set up the environment and build the project

- In the root directory

```
mkdir obj
cd lib && make
cd ..
make
cp db/test-full32.db test.db
```


- Two sample databases have been provided for testing. One contains batches
  composed of exactly one merkle tree. The other is for non full batches, to
  show the verification process when a surveillance period requires data that is
  not does not encompass a full merkle tree.
- The sample database includes 8 users, with 5 days worth of data. There are 32
  entries of data, creating one merkle tree , and each data size is 10 bytes, to
  keep the size of the database file small.

```
cd dbstarter
make
{ optional: alter names.txt }
./dbstarter --user
./dbstarter --data { # now of days of survaillance } { Size of each data entry }

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



