# SCARA Robotic Arm with Graphical Interface

## GRAPHICAL INTERFACE

The graphical interface was programmed in Python using the Flet framework. It is a dynamic interface that communicates with the application via serial communication, displaying real-world changes in the application.

On the left side, the interface features two 2x2 matrices. When a cube is placed in a designated spot within these matrices, the corresponding indicator light for that cube position will illuminate.

The process will automatically pause once both matrices are filled. The operator must then manually restart the process.

A central indicator displays whether the currently detected cube is "Defective" or "Valid". Below this indicator, simple control buttons with essential functions are provided: START, Pause, Continue, and Restart.

![image](https://github.com/user-attachments/assets/b47c6033-fc23-441d-bae3-95be70db0f62)

## SOLIDWORKS ASSEMBLY

![image](https://github.com/user-attachments/assets/763f2afd-31c3-489d-bdb9-a54f1b055ba8)

## ASSEMBLED ROBOT

The microcontroller has two initial conditions. The first is the I2C address of the RGB sensor, which is used for the homing procedure.

Once the homing sequence is complete, the graphical interface needs to send a "START" command to initiate the detection process.

The robot's direct kinematics have been solved to allow for individual joint movement in degrees. This facilitates easier access and modification of specific points. The robot is programmed to visit nine key locations: the eight positions corresponding to the cube locations within the two matrices, and one final position on the conveyor belt for placing the valid cubes. To achieve this, the position vectors for each desired location are calculated and stored within the robot's control code.

If a cube is determined to be valid, the robot will retrieve it from the end of the conveyor belt and place it in its designated spot within the matrix. If a cube is defective, the robot will not proceed until the defective cube is removed and a valid cube arrives. This entire process is managed using simple boolean flags within the control logic.

![image](https://github.com/user-attachments/assets/c74df293-4193-46a2-bcd7-f374a48fad59)
![image](https://github.com/user-attachments/assets/810a3a2f-6350-4f80-82d2-7c47eaa8a196)

## INTERFACE IN OPERATION

![image](https://github.com/user-attachments/assets/bd5ca817-e123-4d63-8c4e-d8d6bd307309)
![image](https://github.com/user-attachments/assets/150f2b4e-0d01-45b9-88c7-3d1d437973cb)
