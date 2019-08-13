

from charm.toolbox.eccurve import prime192v1
from charm.toolbox.eccurve import prime192v2
from charm.schemes.pksig.pksig_ecdsa import ECDSA
from charm.toolbox.ecgroup import ECGroup,ZR,G
from charm.core.engine.util import objectToBytes,bytesToObject

import hashlib
import json


# sign(to_sign)

with open("id_proof.txt",'r') as in_file:
    id_proof = json.load(in_file)


with open("to_sign.txt",'r') as in_file:
    orig = in_file.read()

with open("sig.txt",'r') as in_file:
    sig = in_file.read()

with open("p_priv.txt",'r') as in_file:
    P_priv = in_file.read()

with open("p_pub.txt",'r') as in_file:
    P_pub = in_file.read()


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
    print("id proof is good")

# ??? is needed ???
i = (B ** z)
j = ((C ** s) * y2)


pk = bytesToObject(P_pub.encode('UTF-8'),group)
sig =  bytesToObject(sig.encode('UTF-8'),group)

hashed_data = hashlib.md5(bytes(orig,"utf-8")).hexdigest()

try:
    if (pksign.verify(pk, sig, hashed_data)):
        print("!!! is good")
    else:
        print("no good")
except:
    # invalid siganture 
    print("Error somewhere")



