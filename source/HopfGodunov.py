import numpy as np
import matplotlib.pyplot as plt

FILENAME = "HGudarnaya.txt"

def myFunc(x_parameter):
    return np.sin((4 * np.pi * x_parameter) / (20))


def buildProections(firsQuant, secondQuant, stair, size):
    proectionsArray = []
    for i in range(stair):
        proectionsArray.append(firsQuant)
    for i in range(size - stair):
        proectionsArray.append(secondQuant)
    return proectionsArray

def buildPProections(firsQuant, secondQuant, stairUp, stairDn, size):
    proectionsArray = []
    for i in range(stairUp):
        proectionsArray.append(firsQuant)
    for i in range(stairDn - stairUp):
        proectionsArray.append(secondQuant)
    for i in range(size - stairDn):
        proectionsArray.append(firsQuant)
    return proectionsArray


def hopfGodunov(beginProections, hParameter, tauParameter, endTime, dtauParam=1):
    proectionsMesh = []
    currProections = beginProections.copy()
    proectionsMesh.append(currProections.copy())
    currTime = 0
    countParameter = 0
    while currTime < endTime:
        prevProection = currProections[0]
        #currProections[0] = 0
        for j in range(1, len(beginProections) - 1):
            newProection = currProections[j]
            # Поток слева
            if currProections[j] <= prevProection:
                if currProections[j] + prevProection < 0:
                    fPrev = currProections[j] * currProections[j] / 2
                else:
                    fPrev = prevProection * prevProection / 2
            else:
                if prevProection >= 0:
                    fPrev = prevProection * prevProection / 2
                elif currProections[j] <= 0:
                    fPrev = currProections[j] * currProections[j] / 2
                else:
                    fPrev = 0
            # Поток справа
            if currProections[j] >= currProections[j + 1]:
                if currProections[j] + currProections[j + 1] < 0:
                    fPost = currProections[j + 1] * currProections[j + 1] / 2
                else:
                    fPost = currProections[j] * currProections[j] / 2
            else:
                if currProections[j] >= 0:
                    fPost = currProections[j] * currProections[j] / 2
                elif currProections[j + 1] <= 0:
                    fPost = currProections[j + 1] * currProections[j + 1] / 2
                else:
                    fPost = 0
            currProections[j] = currProections[j] + (tauParameter / hParameter) * (fPrev - fPost)
            prevProection = newProection
        #currProections[-1] = 0
        currTime += tauParameter
        countParameter += 1
        """if countParameter >= dtauParam:
            with open(FILENAME, "a") as f:
                for point in currProections:
                    f.write(str(point) + " ")
                f.write("\n")
                f.write(str(currTime))if countParameter >= dtauParam:
            with open(FILENAME, "a") as f:
                for point in currProections:
                    f.write(str(point) + " ")
                f.write("\n")
                f.write(str(currTime))
                f.write("\n")
            countParameter = 0
                f.write("\n")
            countParameter = 0"""
        proectionsMesh.append(currProections.copy())
    return proectionsMesh


proections = [0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0]
proectionsDown = buildPProections(0, 6, 75, 210, 1350)


fig, ax = plt.subplots(figsize=(16, 9))

z = hopfGodunov(proectionsDown, 0.06,0.006, 60, dtauParam=1)
x = np.linspace(0, 50, 1350)
t = np.linspace(0, 60, 10001)

ax.pcolormesh(x, t, z)
ax.set_xlabel('x', loc='center', fontsize=10)
ax.set_ylabel('t', loc='center', fontsize=10)
ax.set_title('tau =0.006, h=0.06', loc='center', fontsize=15)

fig.savefig("HGudarnaya5060.png", dpi=500)

plt.show()