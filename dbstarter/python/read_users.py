import json

with open("useraccounts.json","r") as f:
    x = json.load(f)


for i in x:
    print(i['R_i'])
