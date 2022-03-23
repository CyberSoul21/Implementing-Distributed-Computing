# Ejemplo de programación con MPI

En esta carpeta están dos ejemplos de paralelización de programas. Los programas están escritos en c y c++,
la paralelización del código se realiza usando mpi (message passing interface). Los ejemplos utilizan el método montecarlo
el cual es un método que por medio de la estadística y la probabilidad se puede determinar valores o soluciones aproximadas a los resultados que buscamos.

## Contenido del repositorio

**Cálculo de pi de forma secuencial**
- pi_mc_cpu.c

**Ejemplo 1 de paralelización del cálculo de pi**
- pi_mc_mpi_exp1.c

**Ejemplo 2 de paralelización del cálculo de pi**
- pi_mc_mpi_exp2.c

**Simulaciónn lanzamiento dado**
- dado.cpp

**Simulación lanzamiento dado paralelizado**
- dadoMPI.cpp

**Archivo Makefile**
- Makefile

## Cálculo de pi de forma secuencial
Se considera un círculo unitario de radio uno (r=1) dentro de de un cuadrado. Sise escoge
un punto al azar que se encuentre dentro del círculo unitario la probabilidad 
de que este punto se encuentre dentro del círculo unitario se da como la proporción entre
el área del círculo unitario y el cuadrado.

![equation](https://latex.codecogs.com/gif.latex?P(x^{2}&space;&plus;&space;y^{2}&space;<&space;1)&space;=&space;\frac{A_{cir}}{A_{cud}}&space;=&space;\frac{\pi}{4})

Si escogemos puntos al azar  N veces y M de esas veces el punto cae dentro del círculo unitario,  la probabilidad de que un punto al azar  caiga  dentro  de  dicho círculo estará dado por:

![equation](https://latex.codecogs.com/gif.latex?P(x^{2}&space;&plus;&space;y^{2}&space;<&space;1)&space;=&space;\frac{M}{N})


Para el cálculo secuencial se genera dos puntos al azar (x,y) un determinado número de veces (número de iteraciones), 
luego se verifica que esten dentro del círculo unitario, si está, es sumado a una variable auxiliar. 

## Ejemplo 1 de paralelización del cálculo de pi

Este programa se ejecuta en todos los procesos que se le indiquen, con un número de iteraciones para cada uno, el mismo en este caso
al finalizar el ciclo for en el cuál se hace la simulación de monte carlo para el cálculo de la cantidad de veces que el punto generado quedó dentro del círculo; este contedo
se almacena en la variable count. Así para optener los conteos de cada proceso se hace uso de la función MPI_Reduce de la librería mpi.h.


## Ejemplo 2 de paralelización del cálculo de 

## Simulación lanzamiento dado paralelizado

## Ejecución de los ejemplos



