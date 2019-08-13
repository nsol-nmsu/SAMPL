
# whats generated?
# 1. id prof
# 2. P_priv
# 3. P_pub


# written out so c can read


# charm includes
from charm.toolbox.eccurve import prime192v1
from charm.toolbox.eccurve import prime192v2
from charm.schemes.pksig.pksig_ecdsa import ECDSA
from charm.toolbox.ecgroup import ECGroup,ZR,G
from charm.core.engine.util import objectToBytes,bytesToObject
import json


# set the account and gen the keys
group = ECGroup(prime192v1)

pksign = ECDSA(group)
g = group.random(G)
a, b = group.random(ZR), group.random(ZR)
A = (g ** a)
B = (g ** b)

C = (g ** (a*b))

#r <- Z_q 
r = group.random(ZR)

#y1 = g^r, y2 = g^{rb} 
y1 = (g ** r)
y2 = (g ** (r * b))

#C's Challenge for I 
s = group.random(ZR)	##THIS SHOULD BE SENT BY C AS A CHALLENGE

# ???
q = group.order()

#I computes z  
z = ((a * s) + r) % q

#String up proof for verification (y1, y2, g, A, B, C, z, s) 
s_y1 = objectToBytes(y1, group).decode('UTF-8')
s_y2 = objectToBytes(y2, group).decode('UTF-8')
s_A = objectToBytes(A, group).decode('UTF-8')
s_B = objectToBytes(B, group).decode('UTF-8')
s_C = objectToBytes(C, group).decode('UTF-8')
s_g = objectToBytes(g, group).decode('UTF-8')
s_z = objectToBytes(z, group).decode('UTF-8')
s_s = objectToBytes(s, group).decode('UTF-8')

# ID PROOF goes here 
id_proof = json.dumps({'s_y1': s_y1, 's_y2': s_y2, 's_A': s_A, 's_B': s_B, 's_C': s_C, 's_g': s_g, 's_z': s_z, 's_s': s_s})

##### write the id proof here
with open("id_proof.txt",'w') as out_file:
    out_file.write(id_proof)


#Structuring pk_AI tied to a,A
pkAI = {'g': g, 'y': A}
skAI = a


#Structuring pk_PI tied to b,B
pkPI = {'g': g, 'y': B}
skPI = b

#A_priv = objectToBytes(skAI, group).decode('UTF-8') 
#A_pub  = objectToBytes(pkAI, group).decode('UTF-8') 


# generate key pair
P_priv = objectToBytes(skPI, group).decode('UTF-8')
P_pub  = objectToBytes(pkPI, group).decode('UTF-8')

##### write out key pair
with open("p_priv.txt",'w') as out_file:
    out_file.write(P_priv)

with open("p_pub.txt",'w') as out_file:
    out_file.write(P_pub)





string_priv_key = P_priv 
string_pub_key  = P_pub 




#return string_priv_key, string_pub_key, self.encryption_key



