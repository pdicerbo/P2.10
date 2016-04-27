import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("MyData.dat")

NPoints = data[:,0]
AvgErr  = data[:,1]
RealErr = np.zeros(len(AvgErr))

xmin = -10.
xmax = 10.

a = np.linspace(xmin, xmax)
j = 0

for my_np in NPoints:
    MyN = int(my_np)
    TmpErr = 0.
    MyGrid = np.linspace(xmin, xmax, int(MyN))

    print("Case of ", MyN, " points")
    for index in np.arange(MyN-1):
        TmpErr += 0.125*np.exp(MyGrid[index+1])*(MyGrid[index+1] - MyGrid[index])**2

    RealErr[j] = TmpErr / my_np
    j += 1
    

plt.figure()
plt.loglog(NPoints, RealErr, label = "Theoretical Upper Bound")
plt.loglog(NPoints, AvgErr,  label = "Obtained Error")
plt.xlabel("Number of Points")
plt.ylabel("Error")
plt.legend()
# plt.show()
plt.savefig("ErrCheck.png")
