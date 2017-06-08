import matplotlib.pyplot as plt
import numpy as np

x = [1, 2, 3, 4, 5, 6, 6.2, 6.5, 7]
y = [0.000001090712761, 0.000001095024412, 0.000001087331639,
	 0.000001077682041, 0.000001065563301, 0.000001114876344,
	 0.000001363838836, 0.000005959040155, 0.000009076444945]

for i,yy in enumerate(y):
	y[i] = yy * 10**7

plt.plot(x, y)

plt.xlabel('Accessed file size in GB')
plt.ylabel('Average I/O Read time per block')
plt.title('File cache experiment')
plt.grid(True)
plt.savefig("file.png")
plt.show()

