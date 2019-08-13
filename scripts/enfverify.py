import matplotlib.pyplot as plt
import pylab


# each entry in the array is for num_users
e_time_5_days=([0.25,0.96,0.92,1.8])
e_time_10_days=([0.72,1.28,2.0,3.5])
e_time_20_days=([1.16,2.2,3.74,6.63])
e_time_50_days=([3.04,4.87,7.63,12.83])

2_time_5_days=([0.28,0.48,0.71,1.38])
2_time_10_days=([0.48,0.9,1.3,2.6])
2_time_20_days=([0.89,1.73,2.5,4.98])
2_time_50_days=([])


####################################################
####################################################

num_users = ([5,10,15,30])

fig, ax = plt.subplots()
ax.plot(num_users,e_time_5_days,linestyle="dotted",label='Surveillance Period - 5 days ( 75K )')
ax.plot(num_users,e_time_10_days,linestyle="dotted",label='Surveillance Period - 10 days ( 75K )')
ax.plot(num_users,e_time_20_days,linestyle="dotted",label='Surveillance Period - 20 days ( 75K )')
ax.plot(num_users,e_time_50_days,linestyle="dotted",label='Surveillance Period - 50 days ( 75K )')


ax.plot(num_users,2_time_5_days,label='Surveillance Period - 5 days ( 2M )')
ax.plot(num_users,2_time_10_days,label='Surveillance Period - 10 days ( 2M )')
ax.plot(num_users,2_time_20_days,label='Surveillance Period - 20 days ( 2M )')
ax.plot(num_users,2_time_50_days,label='Surveillance Period - 50 days ( 2M )')

ax.legend(loc='upper left')

ax.set(xlabel="Number of Users",ylabel="Time (sec)",title="Verification of SRR by Enforcers")

ax.grid()

fig.savefig('SRRverify.png')

