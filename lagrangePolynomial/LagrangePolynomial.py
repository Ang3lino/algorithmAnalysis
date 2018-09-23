
class LagrangePolynomial:

    def __init__(self):
        self.vector = list()
        self.points = list()
        self.degree = 0

    def __lagrange_coefficient(self, k, x):
        n = self.degree
        prod = 1
        xk = self.points[k][0]
        for i in range(n + 1):
            if i != k:
                xi = self.points[i][0]
                prod *= (x - xi) / (xk - xi)
        return prod
    
    def set_points(self, x, y):
        ''' {x_i, y_i}_{i=0}^n-1 '''
        self.points = list(zip(x, y))
        self.degree = len(self.points) - 1

    def evaluate(self, x):
        n = self.degree
        sum = 0
        for i in range(n + 1):
            yi = self.points[i][1]
            sum += yi * self.__lagrange_coefficient(i, x)
        return sum