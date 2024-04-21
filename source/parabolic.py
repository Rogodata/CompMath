import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

FILENAME = "r.txt"
PHOTONAME = "parabolic.png"
DOTNUMBER = 60
LENGTH = np.pi
TIME = 1
TIMEPARAM = 0.005
WITHREAL = 1
DENOMINATOR = 1.1
D3 = 0


####### Неоднородности - функции (x,t)
####### Правильно составляет матрицу и столбец свободных коэффициентов (на отдельных простых случаях точно)
####### Сходится к решению (вроде как) (ДА!)
####### В дальнейшем ошибку смотрим в последнем по времени сечении в нулевой точке
####### Почему то у этого только первый порядок аппроксимации

def realSolution1(x_par, t_par):
    result = x_par * t_par + np.pi * t_par / 2
    # result = np.pi * t_par / 2
    for k_par in range(5000):
        result += (-4 / (np.pi * (2 * k_par + 1) ** 4)) * (
                1 - np.exp(-(2 * k_par + 1) * (2 * k_par + 1) * t_par)) * np.cos((2 * k_par + 1) * x_par)
    return result


def realSolution2(x_par, t_par):
    return np.exp(-t_par) * np.sin(x_par)


def beginFunction(x_parameter):
    return np.cos(2 * x_parameter)


def parFuncOne(par):
    return par


def neodnorodnost(x_parameter, t_parameter):
    return 2 * x_parameter


def zeroFunc(parx, part):
    return 0


def zeroFuncOne(par1):
    return 0


def oneFuncOne(par1):
    return 1


def buildProections(dotNum, lengthParameter, startFunc):
    proectionsArray = [startFunc(0)]
    for i in range(1, int(dotNum + 1)):
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
    # Неоднородность в формате F(x, t)
    timeMesh = []
    proectionsMesh = []
    currProections = beginProections.copy()
    proectionsMesh.append(currProections.copy())
    currTime = 0
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
    # print(a)
    # print(b)
    # print(c)
    while currTime < endTime:
        # Собираем только столбец свободных коэффициентов
        d1 = currProections[1] + courant * (currProections[2] - 2 * currProections[1] + currProections[0]) / (
            2) + tauParameter * (
                     fFunction(xBegin + hParameter, currTime + tauParameter) + fFunction(xBegin + hParameter,
                                                                                         currTime)) / 2

        d = [borderParameters.gamma1(currTime + tauParameter) - borderParameters.beta1 * d1 / (hParameter * courant)]
        for j in range(len(beginProections) - 2):
            d.append(currProections[j + 1] + courant * (
                    currProections[j + 2] - 2 * currProections[j + 1] + currProections[j]) /
                     2 + tauParameter * (
                             fFunction(xBegin + hParameter * (j + 1), currTime + tauParameter) + fFunction(
                         xBegin + hParameter * (j + 1), currTime)) / 2)
        d.append(
            borderParameters.gamma2(currTime + tauParameter) + d[-1] * borderParameters.beta2 / (hParameter * courant))
        # print("d", d)
        # print(currProections)
        currProections = tridiagonalSolver(a, b, c, d)
        #print(currTime)
        timeMesh.append(currTime)
        currTime += tauParameter
        proectionsMesh.append(currProections.copy())
    return (proectionsMesh, timeMesh)


example1 = BorderEquations(0, 0, 1, 1, parFuncOne, parFuncOne)

mesh = []

err = []
num = []
number = 1

for counter in range(40):
    (begin, hParam) = buildProections(DOTNUMBER, LENGTH, zeroFuncOne)
    mesh, times = KrankNikolson(1, neodnorodnost, begin, 0, example1, hParam, TIMEPARAM, TIME)
    # Будем смотреть крайний левый узел сетки
    # realGrid = []
    # x = np.linspace(0, LENGTH, DOTNUMBER + 1)
    # z = mesh[-1]
    # time = times[-1]
    # for point in x:
    #     realGrid.append(realSolution1(point, time))
    # error = np.max(np.abs(np.array(z) - np.array(realGrid))) / np.max(np.abs(np.array(z)))
    error = np.abs(mesh[-1][0] - realSolution1(0, times[-1]))
    print(error)
    err.append(error)
    TIMEPARAM = TIMEPARAM / DENOMINATOR
    DOTNUMBER = int(DOTNUMBER * DENOMINATOR)
    number = number / DENOMINATOR
    print(counter, TIMEPARAM, DOTNUMBER)
    num.append(TIMEPARAM)

fig, ax = plt.subplots(figsize=(16, 9))
k1, cov1 = np.polyfit(np.log(num), np.log(err), 1, cov=True)
psip1 = np.polyval(k1, np.log(num))
ax.plot(np.log(num),np.log(err))
ax.grid(which="major", linewidth=0.5)
ax.grid(which="minor", linestyle='--', linewidth=0.25)
plt.minorticks_on()
ax.plot(np.log(num), psip1, color="black",
        label='ln(err) = ' + '({:.3f} $\pm$ {:.3f})'.format(k1[0], np.sqrt(
            cov1[0][0])) + '$\cdot$ln(h) + ({:.3f}$ \pm$ {:.3f})'.format(k1[1], np.sqrt(cov1[1][1])))
ax.set_title('Зависимость логирифма абсолютной ошибки от логарифма шага', loc='center', fontsize=15)
ax.set_xlabel('ln(h)', loc='center', fontsize=10)
ax.set_ylabel('ln(err)', loc='center', fontsize=10)
ax.legend(loc='upper left', fontsize=10)













# if D3:
#     # пересосздадим массивы для отображения в 3D
#     xGrid = []
#     tGrid = []
#     zGrid = []
#
#     for pointArray in mesh:
#         for point in pointArray:
#             zGrid.append(point)
#
#
#
#     fig = plt.figure()
#     ax = fig.add_subplot(111, projection='3d')
#
#     if WITHREAL:
#         ax.plot_trisurf(xGrid, tGrid, np.array(zGrid) - np.array(realGrid), cmap=cm.coolwarm, linewidth=0,
#                         antialiased=False)
#     else:
#         ax.plot_trisurf(xGrid, tGrid, np.array(zGrid), cmap=cm.coolwarm, linewidth=0, antialiased=False)
#
#     ax.set_xlabel('x', loc='center', fontsize=10)
#     ax.set_ylabel('t', loc='center', fontsize=10)
#
#     fig.savefig(PHOTONAME, dpi=500)
# else:
#     fig, ax = plt.subplots(figsize=(16, 9))
#     z = mesh[-1]
#     time = times[-1]
#     for point in x:
#         if WITHREAL:
#             realGrid.append(realSolution1(point, time))
#     ax.plot(x,z)
#     ax.plot(x,realGrid)


plt.show()

# testBegin = [0.0, 0.00789003986455149, 0.015007747668073529, 0.020656392480018493, 0.02428304572403433, 0.025532705627958188, 0.024283045212764063, 0.02065639314773047, 0.015007747377487932, 0.007890039360820927, 1.3264536899426638e-09, -0.007890041611324144, -0.015007746062055558, -0.02065639358556346, -0.024283045165008742, -0.02553270575096996, -0.024283045389429755, -0.020656392891946516, -0.01500774734664404, -0.007890040025893627, 0.0]

# пробуем однородную задачу Дирихле
# newtest = [1, 2, 2, 3, 3, 2, 2, 1]
# testH = 1
# testTAU = 1
# exampleBorder = BorderEquations(0, 0, 1, 1, parFuncOne, parFuncOne)
# testExampleMesh = KrankNikolson(2, neodnorodnost, newtest, 0, exampleBorder, testH, testTAU, 10)
