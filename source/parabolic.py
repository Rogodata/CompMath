import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

FILENAME = "r.txt"
PHOTONAME = "paraboliczero.png"
DOTNUMBER = 40
LENGTH = np.pi
TIME = 3
TIMEPARAM = 0.05


def beginFunction(x_parameter):
    return np.cos(2 * x_parameter)


def neodnorodnost(x_parameter, t_parameter):
    return (x_parameter + t_parameter)


def zeroFunc(parx, part):
    return 0


def zeroFuncOne(par1):
    return 0


def oneFuncOne(par1):
    return 1


def buildProections(dotNum, lengthParameter, startFunc):
    proectionsArray = [startFunc(0)]
    for i in range(1, dotNum + 1):
        proectionsArray.append(startFunc(lengthParameter * i / dotNum))
    return proectionsArray, lengthParameter / dotNum


class BorderEquations:
    def __init__(self, alpha1, alpha2, beta1, beta2, gamma1, gamma2):
        self.alpha1 = alpha1
        self.alpha2 = alpha2
        self.beta1 = beta1
        self.beta2 = beta2
        self.gamma1 = gamma1
        self.gamma2 = gamma2


def tridiagonalSolver(aArray, bArray, cArray, dArray):
    # Работает
    p = [-cArray[0] / bArray[0]]
    q = [dArray[0] / bArray[0]]
    for index in range(1, len(bArray) - 1):
        yParanmeter = bArray[index] + aArray[index - 1] * p[-1]
        p.append(-cArray[index] / yParanmeter)
        q.append((dArray[index] - aArray[index - 1] * q[-1]) / yParanmeter)
    q.append((dArray[-1] - aArray[-1] * q[-1]) / (aArray[-1] * p[-1] + bArray[-1]))
    for index in range(len(bArray) - 2, -1, -1):
        q[index] += p[index] * q[index + 1]
    return q


def KrankNikolson(aParameter, fFunction, beginProections, xBegin, borderParameters, hParameter, tauParameter, endTime):
    proectionsMesh = []
    currProections = beginProections.copy()
    proectionsMesh.append(currProections.copy())
    currTime = 0
    countParameter = 0
    courant = aParameter * tauParameter / (hParameter * hParameter)
    # матрица собиарется один раз и не меняется
    b = [borderParameters.alpha1 - borderParameters.beta1 / hParameter]
    c = [2 * borderParameters.beta1 / hParameter - borderParameters.beta1 * (courant + 1) / (hParameter * courant)]
    a = []
    for count in range(len(beginProections) - 2):
        a.append(-courant / 2)
        b.append(courant + 1)
        c.append(-courant / 2)
    a.append(
        - 2 * borderParameters.beta2 / hParameter + borderParameters.beta2 * (courant + 1) / (hParameter * courant))
    b.append(borderParameters.beta2 / hParameter + borderParameters.alpha2)
    """print(a)
    print(b)
    print(c)"""
    while currTime < endTime:
        # Собираем только столбец свободных коэффициентов
        d1 = currProections[1] + courant * (currProections[2] - 2 * currProections[1] + currProections[0]) / (
            2) + tauParameter * (
                     fFunction(currTime, xBegin + hParameter) + fFunction(currTime, xBegin + hParameter)) / 2

        d = [borderParameters.gamma1(currTime + tauParameter) - borderParameters.beta1 * d1 / (hParameter * courant)]
        for j in range(len(beginProections) - 2):
            d.append(currProections[j + 1] + courant * (
                    currProections[j + 2] - 2 * currProections[j + 1] + currProections[j]) /
                     2 + tauParameter * (
                             fFunction(currTime + tauParameter, xBegin + hParameter + (j + 1)) + fFunction(currTime,
                                                                                                           xBegin + hParameter * (
                                                                                                                   j + 1))) / 2)
        d.append(
            borderParameters.gamma2(currTime + tauParameter) + d[-1] * borderParameters.beta2 / (hParameter * courant))
        """print(d)
        print(currProections)"""
        currProections = tridiagonalSolver(a, b, c, d)
        print(currTime)
        currTime += tauParameter
        countParameter += 1
        proectionsMesh.append(currProections.copy())
    return proectionsMesh


# testBegin = [0.0, 0.00789003986455149, 0.015007747668073529, 0.020656392480018493, 0.02428304572403433, 0.025532705627958188, 0.024283045212764063, 0.02065639314773047, 0.015007747377487932, 0.007890039360820927, 1.3264536899426638e-09, -0.007890041611324144, -0.015007746062055558, -0.02065639358556346, -0.024283045165008742, -0.02553270575096996, -0.024283045389429755, -0.020656392891946516, -0.01500774734664404, -0.007890040025893627, 0.0]

"""#пробуем однородную задачу Дирихле
newtest = [1,1,1,2,2,1,1,1]
testH = 0.00001
testTAU = 1
exampleBorder = BorderEquations(1, 1, 0, 0, oneFuncOne, oneFuncOne)
testExampleMesh = KrankNikolson(1, zeroFunc, newtest, 0, exampleBorder, testH, testTAU, 100)"""

example1 = BorderEquations(1, 1, 0, 1, zeroFuncOne, zeroFuncOne)
(begin, hParam) = buildProections(DOTNUMBER, LENGTH, beginFunction)
print(hParam)
mesh = KrankNikolson(1, zeroFunc, begin, 0, example1, hParam, TIMEPARAM, TIME)

x = np.linspace(0, LENGTH, DOTNUMBER + 1)
t = np.linspace(0, TIME, int(TIME / TIMEPARAM) + 1)

# пересосздадим массивы для отображения в 3D
xGrid = []
tGrid = []
zGrid = []
for pointArray in mesh:
    for point in pointArray:
        zGrid.append(point)

for time in t:
    for point in x:
        xGrid.append(point)
    for i in range(len(x)):
        tGrid.append(time)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.plot_trisurf(xGrid, tGrid, np.array(zGrid), cmap=cm.coolwarm, linewidth=0, antialiased=False)

ax.set_xlabel('x', loc='center', fontsize=10)
ax.set_ylabel('t', loc='center', fontsize=10)
# ax.set_zlabel('U', loc='center', fontsize=10)

"""#x = np.arange(-2*np.pi, 2*np.pi, 0.2)
y = np.arange(-2*np.pi, 2*np.pi, 0.2)
xgrid, ygrid = np.meshgrid(x,t)
zgrid = np.sin(xgrid) * np.sin(ygrid) / (xgrid * ygrid)
ax.plot_wireframe(xgrid, ygrid, zgrid)"""

fig.savefig(PHOTONAME, dpi=500)

plt.show()
