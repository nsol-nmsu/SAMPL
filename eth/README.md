# Ethereum related files and code
- Start up and helper scripts to deploy a block chain node for each entitiy
- Each entitiy has their own working directory were all their personal ethereum
  information and data lives, such as geth.ipc, for communication, and their
  keystores.


## Getting started ( the ethereum parts )
### Starting ethereum ( locally )
- The scripts in this directory do all the work.
- To start ethereum, locally, with each entitiy having their own node, run
```
./start_all.sh
```
- There should be four ethereum nodes running now, all connected to each other,
  meaning they are syncing their transactions and blocks with each other.

### Stopping ethereum
- For now, to stop all locally running nodes, run
```
killall geth
```
- After they are stopped, it is a good idea to clean slate before next run
```
./clean_all.sh
```

