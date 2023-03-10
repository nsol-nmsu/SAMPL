# write a transaction to the blockchain
import sys

# entity is us, the caller
entity = sys.argv[1]
data = sys.argv[2]
filename = sys.argv[3]

# hard coded address for simple transaction handling
address = 0x5013c86283b5e24d467c926dad63ee61619e50ce


from web3 import Web3,IPCProvider


ent_path = ("eth/" + entity + "/geth.ipc")
# for debugging/testing
#ent_path = ("../../../eth/" + entity + "/geth.ipc")

w3 = Web3(IPCProvider(ent_path,timeout=30))
    
w3.geth.personal.unlockAccount(w3.eth.coinbase,'password1')

tx_hash = w3.eth.sendTransaction({'from':Web3.toChecksumAddress(w3.eth.coinbase),\
        'to':Web3.toChecksumAddress(address),'value':10,'data':bytes(data,'utf-8')})

# write out the hash to read in from c
with open(filename,"wb") as f:
    f.write((tx_hash))
