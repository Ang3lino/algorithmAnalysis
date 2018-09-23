""" 
    La aplicacion corre en python3, en python2 puede fallar. Instalalo con
        sudo apt install python3
    Depende de matplotlib, instalala con:
        pip3 install matplotlib
    Para correr:
        python3 main.py LagrangePolynomial.py 
        python main.py LagrangePolynomial.py (si la primera no funciona)
    Nota: Basta que en una lista de enteros se encuentre un flotante para que 
    cada elemento sea del tipo float.
"""

import matplotlib.pyplot as plt
import numpy as np

from LagrangePolynomial import *
#from random import * # randint function definition

def column(matrix, j):
    col = [ matrix[i][j] for i in range(len(matrix)) ]
    return col

''' The format of the file must be such that between each data has to be an space. '''
data_matrix = np.loadtxt("regvolneg.txt", dtype='f', delimiter=' ')
print(data_matrix)
x_values = range(3, 16)
print(x_values)

for j in range(len(data_matrix[0])):
    lpol = LagrangePolynomial() 
    lpol.set_points(list(x_values), column(data_matrix, j))
    y = [ lpol.evaluate(xi) for xi in x_values ]
    plt.plot(x_values, y, label='fdafda')

tags = ['LM7905', 'LM7912']
plt.title("Regulador de voltaje fijo negativo")
plt.xlabel("Voltaje de la fuente")
plt.ylabel(r'Voltaje en $R_o$')
plt.legend(tags)
plt.grid(True) # cuadriculado
plt.show()
