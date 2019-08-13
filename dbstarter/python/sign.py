# open a file named 'to_sign.txt'
# read it in, sign the content, write signature out to 'sig.txt'


from charm.toolbox.eccurve import prime192v1
from charm.toolbox.eccurve import prime192v2
from charm.schemes.pksig.pksig_ecdsa import ECDSA
from charm.toolbox.ecgroup import ECGroup,ZR,G
from charm.core.engine.util import objectToBytes,bytesToObject

import hashlib


# sign(to_sign)

with open("to_sign.txt",'r') as in_file:
    to_sign = in_file.read()

with open("p_priv.txt",'r') as in_file:
    P_priv = in_file.read()

with open("p_pub.txt",'r') as in_file:
    P_pub = in_file.read()

hashed_data = hashlib.md5(bytes(to_sign,"utf-8")).hexdigest()

group = ECGroup(prime192v1)
pksign = ECDSA(group)
skPI = bytesToObject(P_priv.encode('UTF-8'),group)
pkPI = bytesToObject(P_pub.encode('UTF-8'),group)
r_sig = objectToBytes(pksign.sign(pkPI,skPI,hashed_data),group).decode('UTF-8')

with open("sig.txt","w") as out_file:
    out_file.write(r_sig)

