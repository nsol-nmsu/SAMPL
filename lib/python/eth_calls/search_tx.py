# write a transaction to the blockchain
import sys
import time

# entity is us, the caller
entity = sys.argv[1]


# read this in from tx_hash.txt instead ???
tx_hash = sys.argv[2]


print(f"[{entity}] Searching for tx with hash: '{tx_hash}'")
quit()
"""

from web3 import Web3,IPCProvider

ent_path = ("eth/" + entity + "/geth.ipc")


w3 = Web3(IPCProvider(ent_path,timeout=30))
    
w3.personal.unlockAccount(w3.eth.coinbase,'password1')

while True:
    transaction = w3.eth.getTransaction(tx_hash)
    if transaction is not None:
        print("[*] Found transaction")
        # should write the transaction here..
        # need the format of transaction
        break;
    else:
        time.sleep(2)

"""
