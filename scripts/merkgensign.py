import matplotlib.pyplot as plt
import pylab


####################################################
### std dev
####################################################

####################################################
### means
####################################################

####################################################
### constant
####################################################

num_leaves = ([16,32,64,128,256])

## graphing stuff

fig, ax = plt.subplots()
ax.plot(num_leaves,mean1K,'b-',label='1KB')
ax.plot(num_leaves,mean75K,'r-',label='75KB')
ax.plot(num_leaves,mean1M,'g-',label='1MB')
ax.plot(num_leaves,mean2M,'y-',label='2MB')

ax.legend(loc='upper left')

ax.set(xlabel="Batch Size",ylabel="Time (ms)",title="Time vs Merkle Tree Size")

ax.grid()

fig.savefig('merklegen.png')

