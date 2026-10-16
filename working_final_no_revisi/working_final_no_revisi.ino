#include <NexButton.h>
#include <NexCheckbox.h>
#include <NexConfig.h>
#include <NexCrop.h>
#include <NexDualStateButton.h>
#include <NexGauge.h>
#include <NexGpio.h>
#include <NexHardware.h>
#include <NexHotspot.h>
#include <NexNumber.h>
#include <NexObject.h>
#include <NexPage.h>
#include <NexPicture.h>
#include <NexProgressBar.h>
#include <NexRadio.h>
#include <NexRtc.h>
#include <NexScrolltext.h>
#include <NexSlider.h>
#include <NexText.h>
#include <NexTimer.h>
#include <NexTouch.h>
#include <NexUpload.h>
#include <NexVariable.h>
#include <NexWaveform.h>
#include <Nextion.h>
#include <doxygen.h>

#include <Nextion.h>
#include <Servo.h>
Servo myservo;

// -- led --
int ledOperate = 12;
int ledFinish = 11;

// --- State variables ---
int col = -1, lvl = -1;
bool isStoreOperation = true;
bool save = false;
bool passwordConfirmed = false;  // For store/take confirmation

int x_coords[] = {0, 120, 360, 600}; 
int y_coords[] = {0, 402, 255, 105};
int pos_x = 0; 
int pos_y = 0; 
int last_row = 0; 
int last_column = 0; 

// --- Memory system ---
char name[3][3][4];  // Assuming max 4-character name + null
bool hasName[3][3];
char tempName[4] = "";  // char array with null-terminator, max 4 char (+ null)



// --- Nextion components ---
NexButton b0 = NexButton(3, 2, "b0"); // Yes
NexButton b1 = NexButton(3, 3, "b1"); // No
NexButton b2 = NexButton(4, 3, "b1"); // Done
NexButton sleepButton = NexButton(0, 3, "b3"); // Sleep
NexButton storeButton = NexButton(5, 2, "b0");
NexButton takeButton = NexButton(5, 3, "b1");
NexButton b_1 = NexButton(6, 1, "b0");
NexButton b_2 = NexButton(6, 3, "b1");
NexButton b_3 = NexButton(6, 4, "b2");
NexButton b_4 = NexButton(6, 5, "b3");
NexButton bEnter = NexButton(6, 7, "b5");
NexButton yesSave = NexButton(7, 2, "b0");
NexButton bGoSave = NexButton(6, 9, "b7");
NexButton bGoTake = NexButton(6, 8, "b6");
NexButton bDoneTake = NexButton(8, 3, "b1");
NexButton backPass1 = NexButton(6, 6, "b4");
NexButton backPass2 = NexButton(6, 10, "b8");

NexButton bLvl_1 = NexButton(1, 2, "b1");
NexButton bLvl_2 = NexButton(1, 4, "b2");
NexButton bLvl_3 = NexButton(1, 5, "b3");

NexButton bRow_1 = NexButton(2, 2, "b1");
NexButton bRow_2 = NexButton(2, 5, "b2"); // did you mean 2,5,"b2"
NexButton bRow_3 = NexButton(2, 3, "b3");

// NexPage page0 = NexPage(0, 0, "page0");
// NexPage page1 = NexPage(1, 0, "page1");
// NexPage page2 = NexPage(2, 0, "page2");
// NexPage page3 = NexPage(3, 0, "page3");
// NexPage page5 = NexPage(5, 0, "page5");
// NexPage page6 = NexPage(6, 0, "page6");

NexTouch *nex_listen_list[] = {
  &b0, &b1, &b2, &sleepButton, &storeButton, &takeButton,
  &b_1, &b_2, &b_3, &b_4, &bEnter, &backPass1, &backPass2,
  &yesSave, &bGoSave, &bGoTake, &bDoneTake,
  &bLvl_1, &bLvl_2, &bLvl_3,
  &bRow_1, &bRow_2, &bRow_3,
  NULL
};

// --- Movement Functions ---
void moveLinear(char dir, int val) {
  int stepPin, dirPin, enPin; 
  if (dir == 'x') {
    stepPin = 5; 
    dirPin = 2; 
    enPin = 8; 
  }
  else if (dir == 'y') {
    stepPin = 6; 
    dirPin = 4;
    enPin = 7; 
  }

  digitalWrite(enPin, LOW); 

  float rev = val / 8.0;
  if (val < 0) { digitalWrite(dirPin, LOW); rev *= -1; }
  else { digitalWrite(dirPin, HIGH); }
  int steps = rev * 200;
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH); delayMicroseconds(900);
    digitalWrite(stepPin, LOW); delayMicroseconds(900);      
    myservo.write(90); 
  }

  digitalWrite(enPin, HIGH); 
}

void moveBoth(int valX, int valY) {
  int stepPinX = 5, dirPinX = 2, enPinX = 8;
  int stepPinY = 6, dirPinY = 4, enPinY = 7;

  digitalWrite(enPinX, LOW); 
  digitalWrite(enPinY, LOW); 

  float revX = valX/8.0; 
  float revY = valY/8.0;

  if (valX < 0) {
    digitalWrite(dirPinX, LOW);
    revX *= -1; 
  } else {
    digitalWrite(dirPinX, HIGH);
  }

  if (valY < 0) {
    digitalWrite(dirPinY, LOW);
    revY *= -1; 
  } else {
    digitalWrite(dirPinY, HIGH);
  }

  int stepsX = revX * 200; 
  int stepsY = revY * 200;

  if (stepsX < stepsY) {
    while (stepsX > 0) {
      digitalWrite(stepPinX,HIGH);  
      digitalWrite(stepPinY,HIGH); 
      delayMicroseconds(900); 
      digitalWrite(stepPinX,LOW); 
      digitalWrite(stepPinY,LOW); 
      delayMicroseconds(900); 
      stepsX--; 
      stepsY--; 
      myservo.write(90); 
    }

    while (stepsY> 0) {
      digitalWrite(stepPinY,HIGH); 
      delayMicroseconds(900); 
      digitalWrite(stepPinY,LOW); 
      delayMicroseconds(900); 
      stepsY--; 
      myservo.write(90); 
    }
  }
  else {
    while (stepsY > 0) {
      digitalWrite(stepPinX,HIGH);  
      digitalWrite(stepPinY,HIGH); 
      delayMicroseconds(900); 
      digitalWrite(stepPinX,LOW); 
      digitalWrite(stepPinY,LOW); 
      delayMicroseconds(900); 
      stepsX--; 
      stepsY--; 
      myservo.write(90); 
    }

    while (stepsX > 0) {
      digitalWrite(stepPinX,HIGH); 
      delayMicroseconds(900); 
      digitalWrite(stepPinX,LOW); 
      delayMicroseconds(900); 
      stepsX--; 
      myservo.write(90); 
    }

  }

  digitalWrite(enPinX, HIGH); 
  digitalWrite(enPinY, HIGH); 
}

void moveBothLin(int column, int row) {
  int SENS_X = 50, SENS_Y = 25, DIST_PER_COL = 6, DIST_PER_LVL = 10;
  digitalWrite(ledOperate, HIGH);
  Serial.println("Nyala");

  int x_diff = x_coords[column] - pos_x; 
  int y_diff = y_coords[row] - pos_y; 

  // go to locker
  moveBoth(x_diff, y_diff); 
  pos_x = x_coords[column];  
  pos_y = y_coords[row];

  Serial.print("x_diff: "); 
  Serial.println(x_diff); 
  Serial.print("y_diff: "); 
  Serial.println(y_diff); 

  // take item
  delay(200); 
  servoForward(); 
  delay(200);

  for (int i = 0; i < 10; i++) {
    moveLinear('y', 2); 
    delayMicroseconds(100); 
  }

  delay(200); 
  servoBackward(); 
  delay(200); 

  // give to user
  x_diff = x_coords[2] - pos_x; 
  y_diff = y_coords[2] - pos_y - 20;  
  moveBoth(x_diff, y_diff); 

  servoForward(); 
  delay(200);

  pos_x = x_coords[2]; 
  pos_y = y_coords[2]; 
  last_row = row; 
  last_column = column;

  // --- Visual feedback ---
       
  digitalWrite(ledOperate, LOW);
  Serial.println("Mati");

  digitalWrite(ledFinish, HIGH);
  Serial.print("Nyala selesai");
  delay(2000);
  digitalWrite(ledFinish, LOW);
  Serial.print("Mati");
}

void returnBack() {
  int row = last_row; 
  int column = last_column; 

  int x_diff = x_coords[column] - pos_x; 
  int y_diff = y_coords[row] - pos_y + 20; 

  digitalWrite(ledOperate, HIGH);
  Serial.println("Nyala");

  servoBackward(); 
  delay(200);

  // go to locker
  moveBoth(x_diff, y_diff); 

  // put back
  delay(200); 
  servoForward(); 
  delay(200); 

  for (int i = 0; i < 10; i++) {
    moveLinear('y', -2); 
    delayMicroseconds(100); 
  }

  delay(200); 
  servoBackward(); 
  delay(200); 

  pos_x = x_coords[column];  
  pos_y = y_coords[row];

  digitalWrite(ledOperate, LOW);
  Serial.println("Mati");

  digitalWrite(ledFinish, HIGH);
  Serial.print("Nyala selesai");
  delay(2000);
  digitalWrite(ledFinish, LOW);
  Serial.print("Mati");
}

void servoForward() {
  int pos; 
  for (pos = 90; pos > 0; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    Serial.println(myservo.read()); 
  }
  // delay(1000); 
  myservo.write(90);
  delay(15); 
  // delay(500); 
}

void servoBackward() {
  int pos; 
  for (pos = 91; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    Serial.println(myservo.read()); 
  }
  // delay(1000); 
  delay(100); 
  myservo.write(90);
  delay(15);
  // delay(500);   
}

// --- UI Callbacks ---
void bLvl1PopCallback(void *ptr) { lvl = 0;}
void bLvl2PopCallback(void *ptr) { lvl = 1; no_2();}
void bLvl3PopCallback(void *ptr) { lvl = 2;}

void bRow1PopCallback(void *ptr) { col = 0; confirmSelection(); }
void bRow2PopCallback(void *ptr) { col = 1; confirmSelection(); }
void bRow3PopCallback(void *ptr) { col = 2; confirmSelection(); }

void no_2(){
  Serial.print("page2"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
  Serial.print("vis b2,0"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}

void b0PopCallback(void *ptr) {
  if (lvl < 0 || lvl > 3 || col < 0 || col > 3) return;

  // Decide movement type
  moveBothLin(col+1, lvl+1);
}

void b1PopCallback(void *ptr) { Serial.print("page2"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff); }
void b2PopCallback(void *ptr) { Serial.println("Done"); returnBack(); } //run the done function in takeItem();

void yesSavePopCallback(void *ptr) {
  tempName[0] = '\0';
  Serial.print("page6"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
  Serial.print("vis b4,1"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}

void sleepButtonCallback(void *ptr) {
  Serial.print("thup=1"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
  Serial.print("sleep=1"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}

void backResetPopCallback(void *ptr) {
  tempName[0] = '\0';
}

void resetIfConfirmed() {
  if (passwordConfirmed) {
    tempName[0] = '\0';
    passwordConfirmed = false;
  }
}

void b_1PopCallback(void *ptr) {
  resetIfConfirmed();
  strcat(tempName, "1"); updateSaveField();
}

void b_2PopCallback(void *ptr) {
  resetIfConfirmed();
  strcat(tempName, "2"); updateSaveField();
}
void b_3PopCallback(void *ptr) {
  resetIfConfirmed();
  strcat(tempName, "3"); updateSaveField();
}
void b_4PopCallback(void *ptr) {
  resetIfConfirmed();
  strcat(tempName, "4"); updateSaveField();
}

void updateSaveField() {
  Serial.print("page6"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
  Serial.print("t0.txt=\"");
  Serial.print(tempName);
  Serial.print("\""); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);

  Serial.print(F("Updating t0 with: ")); Serial.println(tempName);  // Debug
}

void bEnterPopCallback() {
  tempName[3] = '\0';  // Ensure null termination

  passwordConfirmed = false;  // Reset by default

  if (!isStoreOperation) {
    for (int l = 0; l < 3; l++) {
      for (int r = 0; r < 3; r++) {
        if (hasName[l][r] && strcmp(name[l][r], tempName) == 0) {
          lvl = l;
          col = r;
          passwordConfirmed = true;

          Serial.print("page6"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
          Serial.print("vis b6,1"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
          return;
        }
      }
    }
    // Name not found
    tempName[0] = '\0';  // Clear input
    Serial.print("page6"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    Serial.print("t0.txt=\"Name not found\""); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);

    // Optional to make button take dissapear when pass -> correct then pass -> false
    Serial.print("page6"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    Serial.print("vis b6,0"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
  }
  else {
    // --- STORE mode ---
    bool isDuplicate = false;

    for (int l = 0; l < 3; l++) {
      for (int r = 0; r < 3; r++) {
        if (hasName[l][r] && strcmp(name[l][r], tempName) == 0) {
          isDuplicate = true;
          break;
        }
      }
      if (isDuplicate) break;
    }

    if (isDuplicate) {
      Serial.print("page6"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
      Serial.print("t0.txt=\"Password taken\""); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
      tempName[0] = '\0';

      Serial.print("vis b7,0"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    } else {
      passwordConfirmed = true;

      Serial.print("page6"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
      Serial.print("vis b7,1"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    }
  }
}

void bGoTakePopCallback(void *ptr) {
  if (!passwordConfirmed) return;
  name[lvl][col][0] = '\0';
  hasName[lvl][col] = false;
  tempName[0] = '\0';
  passwordConfirmed = false;
  moveBothLin(col+1, lvl+1);  // Execute movement
}

void bGoSavePopCallback(void *ptr) {
  if (!passwordConfirmed) return;
  strncpy(name[lvl][col], tempName, 4);
  name[lvl][col][3] = '\0';
  hasName[lvl][col] = true;
  tempName[0] = '\0';
  passwordConfirmed = false;
}

void storeButtonPopCallback(void *ptr) { isStoreOperation = true; }
void takeButtonPopCallback(void *ptr) { 
  isStoreOperation = false; 
  Serial.print("page6"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff); 
  Serial.print("vis b8,1"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff); 
}

// void confirmSelection() {
//   Serial.print("page3"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);

//   Serial.print("t0.txt=\"");  // Start of the message
//   Serial.print(isStoreOperation ? "Store" : "Take");
//   Serial.print("(");
//   Serial.print(lvl+1);
//   Serial.print(",");
//   Serial.print(col+1);
//   Serial.print(")?\"");
//   Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
// }

void confirmSelection() {
  if (hasName[lvl][col]) { // check if already stored
    // ini kalau dia stored == true
    // Kalau error yang b1 or b0, pake ini:

    Serial.print("page3"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    Serial.print("vis b0,0"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    Serial.print("page3"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    Serial.print("vis b1,0"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    Serial.print("page3"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    Serial.print("t0.txt=\"");
    Serial.print("Invalid");
    Serial.print("\""); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);

    // Serial.print("page3"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    // Serial.print("vis b0,0");
    // Serial.print("vis b1,0"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    // Serial.print("page3"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
    // Serial.print("t0.txt=\"");
    // Serial.print("Invalid");
    // Serial.print("\"");
    // Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
  }

  else { // ini kalau dia lvl col nya ga dipake
    Serial.print("page3"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);

    Serial.print("t0.txt=\"");  // Start of the message
    Serial.print(isStoreOperation ? "Store" : "Take");
    Serial.print("(");
    Serial.print(lvl+1);
    Serial.print(",");
    Serial.print(col+1);
    Serial.print(")?\"");
    Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
  }
}

// --- Setup & Loop ---
void setup() {
  int pin_z = 9;
  int stepPinX = 5, dirPinX = 2, enPinX = 8;
  int stepPinY = 6, dirPinY = 4, enPinY = 7;

  Serial.begin(9600);
  delay(500);
  Serial.print("baud=115200"); Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
  Serial.end();
  Serial.begin(115200);
  Serial.println("Start");

  myservo.attach(pin_z);
  pinMode(stepPinX, OUTPUT); pinMode(dirPinX, OUTPUT); pinMode(enPinX, OUTPUT); digitalWrite(enPinX, LOW);
  pinMode(stepPinY, OUTPUT); pinMode(dirPinY, OUTPUT); pinMode(enPinY, OUTPUT); digitalWrite(enPinY, LOW);
  pinMode(ledOperate, OUTPUT); pinMode(ledFinish, OUTPUT);

  // Attach touch events
  b0.attachPop(b0PopCallback); b1.attachPop(b1PopCallback); b2.attachPop(b2PopCallback);
  sleepButton.attachPop(sleepButtonCallback); 
  storeButton.attachPop(storeButtonPopCallback); 
  takeButton.attachPop(takeButtonPopCallback);
  bLvl_1.attachPop(bLvl1PopCallback); bLvl_2.attachPop(bLvl2PopCallback); bLvl_3.attachPop(bLvl3PopCallback); 
  bRow_1.attachPop(bRow1PopCallback); bRow_2.attachPop(bRow2PopCallback); bRow_3.attachPop(bRow3PopCallback);
  b_1.attachPop(b_1PopCallback); b_2.attachPop(b_2PopCallback); b_3.attachPop(b_3PopCallback); b_4.attachPop(b_4PopCallback); 
  bEnter.attachPop(bEnterPopCallback); bGoSave.attachPop(bGoSavePopCallback); 
  bGoTake.attachPop(bGoTakePopCallback);
  yesSave.attachPop(yesSavePopCallback);
  backPass1.attachPop(backResetPopCallback); backPass2.attachPop(backResetPopCallback); 
  bDoneTake.attachPop(b2PopCallback);

  delay(1000); 
  servoForward(); 
  delay(1000); 
  servoBackward();
  delay(200);  
}

void loop() {
  nexLoop(nex_listen_list);
  delay(30);
}