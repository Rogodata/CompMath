import numpy as np


def myFunc(x_parameter):
    return np.sin((4 * np.pi * x_parameter) / (20))


def buildProections(hParameter, spaceSize, startFunc):
    proectionsArray = []
    for i in range(spaceSize + 1):
        proectionsArray.append(startFunc(hParameter * i))
    return proectionsArray


def ugolok(beginProections, timeStopCoord, CFLnum, hParameter, dtauParam=1):
    currProections = beginProections.copy()
    tauParameter = CFLnum * hParameter
    currTime = 0
    countParameter = 0
    while currTime < timeStopCoord:
        prevProection = currProections[0]
        for j in range(1, len(beginProections)):
            newProection = currProections[j]
            currProections[j] = currProections[j] * (1 - CFLnum) + CFLnum * prevProection
            prevProection = newProection
        currProections[0] = currProections[-1]
        currTime += tauParameter
        print("iteration")
        countParameter += 1
        if countParameter >= dtauParam:
            with open("../tests/mainFiles/ugolokVendroff/ug105.txt", "a") as f:
                for point in currProections:
                    f.write(str(point) + " ")
                f.write("\n")
                f.write(str(currTime))
                f.write("\n")
            countParameter = 0
    return currProections


def laxVendroff(beginProections, timeStopCoord, CFLnum, hParameter, dtauParam=1):
    currProections = beginProections.copy()
    tauParameter = CFLnum * hParameter
    currTime = 0
    countParameter = 0
    while currTime < timeStopCoord:
        prevProection = currProections[0]
        firstProection = currProections[1]
        currProections[0] = currProections[0] * (1 - CFLnum * CFLnum) + (- CFLnum / 2 + CFLnum * CFLnum / 2) * \
                            currProections[1] + currProections[-2] * (CFLnum / 2 + CFLnum * CFLnum / 2)
        for j in range(1, len(beginProections) - 1):
            newProection = currProections[j]
            currProections[j] = currProections[j] * (1 - CFLnum * CFLnum) + (- CFLnum / 2 + CFLnum * CFLnum / 2) * \
                                currProections[j + 1] + prevProection * (CFLnum / 2 + CFLnum * CFLnum / 2)
            prevProection = newProection

        currProections[-1] = currProections[-1] * (1 - CFLnum * CFLnum) + (- CFLnum / 2 + CFLnum * CFLnum / 2) * \
                             firstProection + prevProection * (CFLnum / 2 + CFLnum * CFLnum / 2)
        currTime += tauParameter
        print("iteration")
        countParameter += 1
        if countParameter >= dtauParam:
            with open("../tests/mainFiles/ugolokVendroff/LV1.txt", "a") as f:
                for point in currProections:
                    f.write(str(point) + " ")
                f.write("\n")
                f.write(str(currTime))
                f.write("\n")
            countParameter = 0
    return currProections


proections = buildProections(0.5, 40, myFunc)
print(proections)
with open("../tests/mainFiles/ugolokVendroff/ug105.txt", "a") as f:
    for point in proections:
        f.write(str(point) + " ")
    f.write("\n")
    f.write("0")
    f.write("\n")

print(ugolok(proections, 40, 1.05, 0.5, dtauParam=8))
