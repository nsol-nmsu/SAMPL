# what files need ???
# id_proof.txt
# root_hash.txt
# root_sig.txt
# p_pub.txt
# 
# all these need to be written from the c code

from charm.toolbox.eccurve import prime192v1
from charm.toolbox.eccurve import prime192v2
from charm.schemes.pksig.pksig_ecdsa import ECDSA
from charm.toolbox.ecgroup import ECGroup,ZR,G
from charm.core.engine.util import objectToBytes,bytesToObject

import hashlib
import json
import time


#print("[*_*] in python verify stage [*_*]")

with open("id_proof.txt",'r') as in_file:
    id_proof = json.load(in_file)


with open("root_hash.txt",'r') as in_file:
    root_hash = in_file.read()

with open("root_sig.txt",'r') as in_file:
    root_sig = in_file.read()


with open("p_pub.txt",'r') as in_file:
    P_pub = in_file.read()


start = time.time_ns()

group = ECGroup(prime192v1)
pksign = ECDSA(group)
q = group.order()


y1 = bytesToObject(id_proof['s_y1'].encode('UTF-8'), group)
y2 = bytesToObject(id_proof['s_y2'].encode('UTF-8'), group)
A = bytesToObject(id_proof['s_A'].encode('UTF-8'), group)
B = bytesToObject(id_proof['s_B'].encode('UTF-8'), group)
C = bytesToObject(id_proof['s_C'].encode('UTF-8'), group)
g = bytesToObject(id_proof['s_g'].encode('UTF-8'), group)
z = bytesToObject(id_proof['s_z'].encode('UTF-8'), group)
s = bytesToObject(id_proof['s_s'].encode('UTF-8'), group)

i = (g ** z)
j = ((A ** s) * y1)


if ((group.coordinates(i)[0] == (group.coordinates(j)[0] % q)) and \
        (group.coordinates(i)[1] == (group.coordinates(j)[1] % q))) and\
((group.coordinates(i)[0] == (group.coordinates(j)[0] % q)) and\
        (group.coordinates(i)[1] == (group.coordinates(j)[1] % q))):
   print("[*] id proof is good")
   status = 1

# ??? is needed ???
i = (B ** z)
j = ((C ** s) * y2)

end = time.time_ns()
total = round(end-start,6)
with open("id_to_total.txt","a") as f:
    f.write(str(total)+"\n")
#print(f"ID_TIME: {total}")

start = time.time_ns()
pk = bytesToObject(P_pub.encode('UTF-8'),group)
sig =  bytesToObject(root_sig.encode('UTF-8'),group)

hashed_data = hashlib.md5(bytes(root_hash,"utf-8")).hexdigest()

try:
    if (pksign.verify(pk, sig, hashed_data)):
        print("[*] Signature is good")
        status += 1
    else:
        print("[*] Signature is NOT good")
except:
    # invalid siganture 
    print("Error somewhere")

end = time.time_ns()
total = round(end-start,6)
with open("sig_to_total.txt","a") as f:
    f.write(str(total)+"\n")
#print(f"SIG_TIME: {total}")


