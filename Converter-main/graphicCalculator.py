import math
import numpy as np
import matplotlib.pyplot as plot


class mathFunction():
    def __init__(self, lefExtreme, rightExtreme):
        return NotImplementedError
        


def sin(x):
    return math.sin(x)

def tan(x):
    return math.tan(x)

def cos(x):
    return math.cos(x)



def absolute_value(x):
    return abs(x)

def quadratic_function(x,a,b,c):
    return (a*x**2)+(b*x)+c


x = np.linspace(0, 2 * np.pi, 100)
y = np.sin(x)

#plot.plot(x,y)
plot.boxplot([12,12,23,2,1,2])
plot.show()
  