# write a transaction to the blockchain
import sys
import time

# entity is us, the caller
entity = sys.argv[1]
filename = sys.argv[2]
filename_2 = sys.argv[3]


# read this in from tx_hash.txt instead ???
#tx_hash = sys.argv[2]

with open(filename,"rb") as f:
    tx_hash = f.read()


from web3 import Web3,IPCProvider

#ent_path = ("eth/" + entity + "/geth.ipc")
ent_path = ("../../../eth/" + entity + "/geth.ipc")


w3 = Web3(IPCProvider(ent_path,timeout=30))
    
w3.geth.personal.unlockAccount(w3.eth.coinbase,'password1')

while True:
    transaction = w3.eth.getTransaction(tx_hash)
    if transaction is not None:
        print("[*] Found transaction: tx.input listed below...")
        print(bytes.fromhex(transaction.input[2:]).decode('ascii'))
        t = bytes.fromhex(transaction.input[2:]).decode('ascii')
        with open(filename_2,"w") as f:
            f.write(t)

        break;
    else:
        time.sleep(2)





