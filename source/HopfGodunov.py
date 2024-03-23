import numpy as np

FILENAME = "HGup1.txt"

def myFunc(x_parameter):
    return np.sin((4 * np.pi * x_parameter) / (20))


def buildProections(firsQuant, secondQuant, stair, size):
    proectionsArray = []
    for i in range(stair):
        proectionsArray.append(firsQuant)
    for i in range(size - stair):
        proectionsArray.append(secondQuant)
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
        print("iteration")
        countParameter += 1
        if countParameter >= dtauParam:
            with open(FILENAME, "a") as f:
                for point in currProections:
                    f.write(str(point) + " ")
                f.write("\n")
                """f.write(str(currTime))
                f.write("\n")"""
            countParameter = 0
        proectionsMesh.append(currProections.copy())
    return proectionsMesh


proections = [6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0]
proectionsDown = buildProections(6, 1, 5, 36)
proections.reverse()
print(len(proectionsDown))
with open(FILENAME, "a") as f:
    for point in proectionsDown:
        f.write(str(point) + " ")
    f.write("\n")
    """f.write("0")
    f.write("\n")
"""
print(hopfGodunov(proectionsDown, 1,0.1, 5, dtauParam=1))