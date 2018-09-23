import numpy as np
import matplotlib.pyplot as plt

#Ajustador de puntos a polinomio de grado n
def main ():
	grado = 3
	x = np.array([0.0, 1.0, 2.0, 3.0,  4.0,  5.0]) #dominio
	y = np.array([0.0, 0.8, 0.9, 0.1, -0.8, -1.0]) #contradominio
	z = np.polyfit(x, y, grado)	#coeficientes del polinomio ajustado
	p = np.poly1d(z) #polinomio valuable
	xp = np.linspace(-2, 6, 100) #rango a evaluar el polinomio

	_, graficas = plt.subplots() #se declaran las subgraficas que tendra el grafico final
	graficas.plot(x, y, '.', label = 'Puntos') #grafica de puntos
	graficas.plot(xp, p(xp), '-', label = 'Polinomio grado ' + str(grado)) #grafica del polinomio
	
	legend = graficas.legend(loc='upper right', shadow=True, fontsize='medium')
	plt.ylim(-2, 2) #limites de altura de la grafica final
	plt.show() #se muestra en pantalla la grafica final

#Invocacion de main
main()