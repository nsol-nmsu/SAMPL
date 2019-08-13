# read in the two files, calc the averae, and dump to avg.txt

x_75_32 = 0.002856
x_2_32  = 0.003035
x_75_64 = 0.003011
x_2_64  = 0.003224

id_total = 0
with open("id_to_total.txt","r") as f:
    for line in f.readlines():
        id_total += float(line)




sig_total = 0
with open("sig_to_total.txt","r") as f:
    for line in f.readlines():
        sig_total += float(line)


with open("counter.txt","r") as f:
    y = float(f.readline())

root_total = 0
with open("root_to_total.txt","r") as f:
    for line in f.readlines():
        root_total += float(line)

root_total_1 = root_total + ( y * x_75_64 )
root_total_2 = root_total + ( y * x_2_64 )

with open("avg.txt","w") as f:
    f.write(f"ID_TIME: {id_total}\n")
    f.write(f"SIG_TIME: {sig_total}\n")
    f.write(f"ROOT_TIME_75: {root_total_1}\n")
    f.write(f"ROOT_TIME_2: {root_total_2}\n")
