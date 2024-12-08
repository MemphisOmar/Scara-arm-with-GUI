
#include <AccelStepper.h>
#include <Servo.h>///////////pendiente gripper
#include <math.h>
#include <TimerOne.h>

#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 A3
#define limitSwitch4 9

AccelStepper stepper1(1, 2, 5); // (driver, STEP, DIR)
AccelStepper stepper2(1, 3, 6);
AccelStepper stepper3(1, 4, 7); //////// PENDIENTE GRIPPER
AccelStepper stepper4(1, 12, 13);
Servo gripperServo; 

int conveyorBeltPosition[4] = {0, 0, 0, 13500};

const int numPoints = 8;
int points[numPoints][4] = {
  {100, 500, 300, 15400}, // Punto 1
  {-150, -250, 350, 15450}, // Punto 2
  {200, 300, 400, 15500}, // Punto 3
  {-350, -750, 450, 15550}, // Punto 4
  {300, 600, 500, 15600}, // Punto 5
  {-350, -150, 550, 15650}, // Punto 6
  {400, 500, 600, 15700}, // Punto 7
  {-450, -550, 650, 15750}  // Punto 8
};



double x = 10.0;
double y = 10.0;
double L1 = 228; // L1 = 228mm
double L2 = 136.5; // L2 = 136.5mm
double theta1, theta2, phi, z;

int stepper1Position, stepper2Position, stepper3Position, stepper4Position;

const float theta1AngleToSteps = 44.444444;
const float theta2AngleToSteps = 35.555555;
const float phiAngleToSteps = 10;
const float zDistanceToSteps = 100;

byte inputValue[5];
int k = 0;
bool paused = false;
bool startCommandReceived = false;
bool stopCommandReceived = false;
String content = "";
int data[10];
int theta1Array[100];
int theta2Array[100];
int phiArray[100];
int zArray[100];
int gripperArray[100];
int positionsCounter = 0;
volatile bool toggle = false;


void setup() {
  Serial.begin(115200);

  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(limitSwitch3, INPUT_PULLUP);
  pinMode(limitSwitch4, INPUT_PULLUP);

  // Stepper motors max speed
  stepper1.setMaxSpeed(4000);
  stepper1.setAcceleration(2000);
  stepper2.setMaxSpeed(4000);
  stepper2.setAcceleration(2000);
  stepper3.setMaxSpeed(4000);
  stepper3.setAcceleration(2000);
  stepper4.setMaxSpeed(4000);
  stepper4.setAcceleration(2000);
/////////////////////////// pendiente gripper
  gripperServo.attach(A0, 600, 2500);
  data[6] = 180;
  gripperServo.write(data[6]);
  ////////////////////////////////////
  delay(1000);
  data[5] = 100;
  homing();

  
  // Configurar Timer1 para imprimir "VAL" cada 20 segundos
  Timer1.initialize(20000000); // 20 segundos en microsegundos
  Timer1.attachInterrupt(printVal);
}

void loop() {

  /////test def/val
  // Esperar el comando "START" para iniciar
  if (!startCommandReceived) {
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      if (command == "START") {
        startCommandReceived = true;
        stopCommandReceived = false;
        Serial.println("Comando START recibido. Iniciando...");
      }
    }
    return; // No hacer nada hasta recibir el comando "START"
  }


  for (int i = 0; i < numPoints; i++) {
    // Mover a la banda transportadora
    moveToPosition(conveyorBeltPosition);
    delay(1000); // Espera un segundo antes de tomar el cubo

    // Simular la acción de tomar el cubo
    gripperServo.write(90); // Cerrar el gripper
    delay(500); // Espera medio segundo para asegurar que el cubo está tomado

    // Mover al punto de destino
    moveToPosition(points[i]);
    delay(1000); // Espera un segundo antes de soltar el cubo

    // Simular la acción de soltar el cubo
    gripperServo.write(180); // Abrir el gripper
    delay(500); // Espera medio segundo para asegurar que el cubo está soltado

    // Imprimir el punto actual después de visitarlo
    Serial.print("P");
    Serial.println(i + 1);

    // Verificar si se debe pausar
    while (paused) {
      if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        if (command.equals("CONTINUAR")) {
          paused = false;
          Serial.println("CONTINUAO");
        }
      }
    }

    // Verificar si se debe pausar después de cada punto
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      if (command.equals("PAUSAR")) {
        paused = true;
        Serial.println("PAUSAO");
      }
    }
  }

  // Pausar al llegar al último punto y esperar el comando "REINICIAR"
  while (true) {
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      if (command.equals("REINICIAR")) {
        break;
      }
    }
  }
}


void printVal() {
  if (toggle) {
    Serial.println("VAL");
    Timer1.initialize(35000000); // 35 segundos en microsegundos
  } else {
    Serial.println("DEF");
    Timer1.initialize(20000000); // 20 segundos en microsegundos
  }
  toggle = !toggle;
}

void moveToPosition(int point[4]) {
  stepper1.moveTo(point[0]);
  stepper2.moveTo(point[1]);
  stepper3.moveTo(point[2]);
  stepper4.moveTo(point[3]);

  while (stepper1.currentPosition() != point[0] || 
         stepper2.currentPosition() != point[1] || 
         stepper3.currentPosition() != point[2] || 
         stepper4.currentPosition() != point[3]) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
    stepper4.run();
  }
}

void serialFlush() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void homing() {
  // Homing Stepper4
  while (digitalRead(limitSwitch4) != 0) {
    stepper4.setSpeed(1500);
    stepper4.runSpeed();
    stepper4.setCurrentPosition(17000); // When limit switch pressed set position to 0 steps
  }
  delay(20);
  stepper4.moveTo(16500);
  while (stepper4.currentPosition() !=16500) {
    stepper4.run();
  }

  stepper3.moveTo(0);
  while (stepper3.currentPosition() != 0) {
    stepper3.run();
  }

  // Homing Stepper2
  while (digitalRead(limitSwitch2) != 0) {
    stepper2.setSpeed(-1300);
    stepper2.runSpeed();
    stepper2.setCurrentPosition(-5420); // When limit switch pressed set position to -5440 steps
  }
  delay(20);

  stepper2.moveTo(0);
  while (stepper2.currentPosition() != 0) {
    stepper2.run();
  }

  // Homing Stepper1
  while (digitalRead(limitSwitch1) != 0) {
    stepper1.setSpeed(-1200);
    stepper1.runSpeed();
    stepper1.setCurrentPosition(-3955); // When limit switch pressed set position to 0 steps
  }
  delay(20);
  stepper1.moveTo(0);
  while (stepper1.currentPosition() != 0) {
    stepper1.run();
  }
}