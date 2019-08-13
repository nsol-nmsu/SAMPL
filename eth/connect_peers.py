# the arg passed is who we are. So if this is
# being run on the machine that is the company, then
# you would pass company as the argument
# connects all the peers for sync

# !!! before running this, the {entity}-info.txt needs
# to have been shared to all machines because we are going to read them

import os
import json
import time
import socket
import sys
from web3 import Web3,IPCProvider

if len(sys.argv) == 1:
    print("[!] Enter what entity this is")
    quit()

caller = sys.argv[1] # who called this, ie. company

############################################################
# set up the eth

w3 = Web3(IPCProvider(caller+"/geth.ipc"))
w3.personal.unlockAccount(w3.eth.coinbase,'password1')


ents = ["company","law","enforcer","judge"]
for ent in ents:
    attempts = 2
    if ent != caller:
        print("[*] Connecting peer '"+ent+"'")
        with open(ent+"-info.txt","r") as f:
            info = json.load(f)

        enode = info['enode']
        ip = info['ip']
        attempts = 2
        while(attempts > 0):
            status = w3.admin.addPeer(enode)
            if status == True:
                print(f"[*] added peer at:{ip}")
                break;
            else:
                print("[*] trying connect again")

            time.sleep(1)
            attempts = attempts - 1






