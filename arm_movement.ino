# include <Stepper.h>
# include <Servo.h>

# define STEPS_PER_REV 2048
# define IN1 10
# define IN2 11
# define IN3 12
# define IN4 13
Stepper stepperX(STEPS_PER_REV, IN1, IN3, IN2, IN4); // forward/backward movements (stepper)

// side to side movements controlled (servo)
# define SERVO_PIN 9
Servo myServo;
int servoAngle = 90;   
// limited range of motion to avoid collision with chest    
const int SERVO_MIN = 45; 
const int SERVO_MAX = 90;

// Joystick pins
# define VRx A0   // Left/Right
# define VRy A1   // Up/Down

const int DEADZONE = 100;
const int SPEED_MAX = 15;   // Max RPM for stepper

void setup() {
  Serial.begin(9600);
  stepperX.setSpeed(SPEED_MAX);
  myServo.attach(SERVO_PIN);
  myServo.write(servoAngle);
  pinMode(SW, INPUT_PULLUP);
}

// set stepper motor speeds and servo positions based on joystick input, deadzone, offset
void loop() {
  int joyX = analogRead(VRx); 
  int joyY = analogRead(VRy);  

  int offsetX = joyX - 512;
  int offsetY = joyY - 512;

  if (abs(offsetX) < DEADZONE) offsetX = 0;
  if (abs(offsetY) < DEADZONE) offsetY = 0;

  int motorSpeedX = 0;

  if (offsetX != 0) {
    motorSpeedX = map(abs(offsetX), DEADZONE, 512, 2, SPEED_MAX);
    stepperX.setSpeed(motorSpeedX);

    int stepAmountX = map(abs(offsetX), DEADZONE, 512, 20, 100);
    if (offsetX > 0) stepperX.step(stepAmountX);  
    else stepperX.step(-stepAmountX);              
  }

  if (offsetY != 0) {
    int servoMove = map(offsetY, -512, 512, SERVO_MAX, SERVO_MIN); 
    servoMove = constrain(servoMove, SERVO_MIN, SERVO_MAX);
    servoAngle = servoMove;
    myServo.write(servoAngle);
  }

  // debug output to Serial Monitor
  Serial.print("X: "); Serial.print(joyX);
  Serial.print(" | Y: "); Serial.print(joyY);
  Serial.print(" | StepSpeedX: "); Serial.print(motorSpeedX);
  Serial.print(" | ServoAngle: "); Serial.println(servoAngle);
}