import matplotlib.pyplot as plt
import numpy as np

x = [1,2,3,4,5,6,7,8,9,10]
y = [5.779779, 3.968996, 4.397888, 3.379362, 3.502840, 3.598444, 3.544075, 3.609133, 3.957449, 4.321946]
for i, yy in enumerate(y):
	y[i] = yy/10.0

z = [0.006411, 0.006448, 0.006506, 0.006695, 0.006846, 0.007025, 0.006655, 0.006293, 0.006417, 0.006467]
for i, zz in enumerate(z):
	z[i] = zz*10.0

l = [0.013, 0.020, 0.020, 0.019, 0.018, 0.020, 0.017, 0.018, 0.019, 0.017]
p = [2.89, 2.34, 2.36, 2.34, 2.37, 2.32, 2.33, 2.33, 2.32, 2.33]
for i, pp in enumerate(p):
	p[i] = pp/10.0

plt.plot(x, y)
plt.plot(x, z)
plt.plot(x, l)
plt.plot(x, p)

plt.legend(['Remote interface RTT', 'Loopback interface RTT', 'Ping loopback', 'Ping Remote'], loc='best')

plt.xlabel('Iteration number')
plt.ylabel('RTT in microseconds')
plt.title('RTT experiment')
plt.grid(True)
plt.show()