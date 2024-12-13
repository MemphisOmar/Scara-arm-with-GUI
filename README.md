INTERFAZ GRAFICA

La interfaz grafica fue programada en Python con el framework Flet, es una interfaz dinamica donde comunica el serial con la aplicacion, así mostrando cambios en el mundo real.

Cuenta con 2 matrices a el lado izquierdo donde al dejar el cubo en su lugar enciende el indicador del cubo por el cual estemos pasando, son 2 matrices de 2x2.

El proceso se pausara al llenar las 2 matrices hasta que el operario reinicie el proceso.

Un indicador central donde nos dira si el cubo el Defectuoso o valido y botones de control simples con las funciones escensiales. START, Pausar, Continuar y Reiniciar.
![image](https://github.com/user-attachments/assets/b47c6033-fc23-441d-bae3-95be70db0f62)

ENSAMBLE EN SOLIDWORKS
![image](https://github.com/user-attachments/assets/763f2afd-31c3-489d-bdb9-a54f1b055ba8)

ROBOT ENSAMBLADO

El microcontrolador tiene 2 condiciones iniciales, la ubicacion I2C del sensor rgb para hacer el homing.

Una vez finalizado el homing necesitar que la interfaz grafica envíe la orden de "START" para iniciar el proceso de deteccion.

El robot cuenta con una cinematica directa resuelta para mover cada Joint en grados, y así visitar y modificar los puntos con mayor facilidad, los puntos a visitar son los 8 pertenecientes a la ubicacion matricial de los cubos y 1 perteneciente a la ubicacion final del cubo en la banda transportadora, para esto solo necesitamos obtener los vectores de posicion que llegaran a la ubicacion deseada y guardarlos en nuestro codigo.

Si el cubo es valido ira por el al final de la banda y lo acomodara a su lugar asignado en la matriz, si es defectuoso el robot no avanzara hasta que el cubo sea descartado y llegue uno valido, todo esto con simples banderas.

![image](https://github.com/user-attachments/assets/c74df293-4193-46a2-bcd7-f374a48fad59)
![image](https://github.com/user-attachments/assets/810a3a2f-6350-4f80-82d2-7c47eaa8a196)

INTERFAZ FUNCIONANDO
![image](https://github.com/user-attachments/assets/bd5ca817-e123-4d63-8c4e-d8d6bd307309)
![image](https://github.com/user-attachments/assets/150f2b4e-0d01-45b9-88c7-3d1d437973cb)
