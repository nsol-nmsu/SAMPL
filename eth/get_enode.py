# get the enode and port of the running
# eth node of the specified entity

# dump in json


import os
import json
from pathlib import Path
import socket
import sys
from web3 import Web3,IPCProvider

if len(sys.argv) == 1:
    print("[!] For who ??   ./get_enode.py {entity}")
    quit()
name = sys.argv[1] # who called this, ie. company

if Path(name+"/geth.ipc").exists() == False:
    print("[!] There is no geth.ipc")
    print("[!] Start the eth node first!")
    print("[!] Quitting")
    quit()




###############################################################
###############################################################


sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.connect(("8.8.8.8",80))
host_ip = sock.getsockname()[0]

sock.close()

# set the path here for each entity
w3 = Web3(IPCProvider( name + "/geth.ipc"))
w3.personal.unlockAccount(w3.eth.coinbase,'password1')
enode = w3.admin.nodeInfo.enode
s = enode.split('@')
new_enode =  s[0] + "@" + host_ip

if name == "judge":
    new_enode = new_enode + ":30303"
elif name == "company":
    new_enode = new_enode + ":30304"
elif name == "law":
    new_enode = new_enode + ":30305"
elif name == "enforcer":
    new_enode = new_enode + ":30306"

to_print = {'enode':new_enode,'ip':host_ip}

print("[*] Printing out endoe and ip info to '" + name + "-info.txt' in json")
with open(name+"-info.txt","w") as f:
    json.dump(to_print,f)



###############################################################
###############################################################



