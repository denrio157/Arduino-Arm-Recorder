#include <Servo.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;


#define sw1 10
#define sw2 11
#define r_led 8
#define g_led 9

#define sensor1 A0
#define sensor2 A1
#define sensor3 A2
#define sensor4 A3


int vsensor1 = 0;
int vsensor2 = 0;
int vsensor3 = 0;
int vsensor4 = 0;

int ovsensor1 = 0;
int ovsensor2 = 0;
int ovsensor3 = 0;
int ovsensor4 = 0;


unsigned char vlog1[250];
unsigned char vlog2[250];
unsigned char vlog3[250];
unsigned char vlog4[250];

bool osw1=0;
bool osw2=0;

unsigned long swdebto=100ul;
unsigned long sw1deb=millis()+swdebto;
unsigned long sw2deb=millis()+swdebto;

byte rec_size = 1000;
unsigned long rec_rate = 50;
unsigned long rec_rate_counter = 0;
bool rec_active = 0;
bool play_active = 0;
unsigned char rec_counter = 0;
bool blinker;
unsigned long rec_blink_rate = 300ul;
unsigned long rec_blink_ratecounter = millis() + rec_blink_rate;

#Servo Move
void setup() {
  com1.begin(19200);
  com1.println("Micro Arm Started");
  myservo1.attach(3);
  myservo2.attach(4);
  myservo3.attach(5);
  myservo4.attach(6);
  myservo4.write(155);
  
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  
  pinMode(r_led, OUTPUT);
  pinMode(g_led, OUTPUT);

  digitalWrite(r_led, LOW);
  digitalWrite(g_led, LOW);

  digitalWrite(sw1, HIGH);
  digitalWrite(sw2, HIGH);
  
  delay(1000);
  //tepukTangan(10);
  //beriHormat();
}

void playing() {
  digitalWrite(g_led, HIGH);
  Serial.print("Start playing ... (");
  Serial.print(rec_counter);
  Serial.println(')');  
  for (int i=0; i<rec_counter-1; i++) {
    Serial.print("Step: ");
    Serial.println(i);
    moveServos(vlog1[i], vlog2[i], vlog3[i], vlog4[i]);  
    //delay(rec_rate);
    delay(500);
    readSW();
    if (play_active==0) break;
  }
  digitalWrite(g_led, LOW);
  Serial.println("Stop playing ...");
}
void recording(byte s1, byte s2, byte s3, byte s4) {
  if (rec_active==1) {
    if (millis() > rec_blink_ratecounter) {
      rec_blink_ratecounter = millis() + rec_blink_rate;
      if (blinker) 
        digitalWrite(r_led, LOW); else digitalWrite(r_led, HIGH);
      blinker=!blinker;
    }
    
    /*if (millis() > rec_rate_counter) {
      vlog1[rec_counter] = s1;
      vlog2[rec_counter] = s2;
      vlog3[rec_counter] = s3;
      vlog4[rec_counter] = s4;
      Serial.print("Rec step ... ");
      Serial.println(rec_counter);
      rec_counter++;
      if (rec_counter>rec_size-1) {
        digitalWrite(r_led, LOW);
        Serial.println("Stop recording ...");
        rec_active = 0;
      } else {
        
      }
      rec_rate_counter = millis() + rec_rate;
    }*/
  }
}

void sw1action() {  
  if (rec_active==0) {    
    play_active = 0;
    digitalWrite(g_led, LOW);
    rec_counter = 0;
    Serial.println("Start recording ...");
    rec_active = 1; 
  } else {
    Serial.println("Stop recording ...");
    digitalWrite(r_led, LOW);
    rec_active = 0;
  }
}

void sw2action() {
  digitalWrite(g_led, HIGH);  
  if (rec_active==1) {
    vlog1[rec_counter] = 180-vsensor1;
    vlog2[rec_counter] = 180-vsensor2;
    vlog3[rec_counter] = vsensor3;
    vlog4[rec_counter] = vsensor4;
    Serial.print("Rec step ... ");
    Serial.println(rec_counter);
    rec_counter++;
  } else if (play_active==0) {
    rec_active = 0;
    play_active = 1;
  } else {
    play_active = 0;
  }  
}

void sw3action() {
  tepukTangan(10);
}

void readSW() {
  bool vsw1 = digitalRead(sw1);
  bool vsw2 = digitalRead(sw2);

  if (millis()>sw1deb) {
    if (vsw1!=osw1) {    
      osw1 = vsw1;
      if (vsw1==0) {
        sw1action();
      }
    }
    sw1deb=millis() + swdebto;
  }

  if (millis()>sw2deb) {
    if (vsw2!=osw2) {
      osw2 = vsw2;
      if (vsw2==0) {        
        if (vsw1==LOW) {
          sw3action();
        } else sw2action();
      }
    }
    sw2deb=millis() + swdebto;
  }
  //if (
}

void tepukTangan(int count) {
  moveServos(90, 75, 162, 155); delay(1000); 
  
  for (int i = 0; i < count; i++) {
    readSW();
    myservo4.write(140);delay(120);
    myservo4.write(175);delay(120);
  }
}

void beriHormat() {
  moveServos(90, 75, 162, 141); delay(1000); 

  for (int i=0; i<45; i++) {
    moveServos(90-i, 75, 162, 141); delay(5); 
  }
  for (int i=0; i<45; i++) {
    moveServos(45, 75-i, 162, 141); delay(20);
  }
  for (int i=0; i<45; i++) {
    moveServos(45, 30+i, 162, 141); delay(10);
  }
  
  for (int i=0; i<90; i++) {
    moveServos(45+i, 75, 162, 141); delay(5); 
  }
  for (int i=0; i<45; i++) {
    moveServos(135, 75-i, 162, 141); delay(20);
  }
  for (int i=0; i<45; i++) {
    moveServos(135, 30+i, 162, 141); delay(10);
  }

  for (int i=0; i<45; i++) {
    moveServos(135-i, 75, 162, 141); delay(5); 
  }
  for (int i=0; i<45; i++) {
    moveServos(90, 75-i, 162, 141); delay(20);
  }
  for (int i=0; i<45; i++) {
    moveServos(90, 30+i, 162, 141); delay(10);
  }

  delay(1500);
  //x,30,162,x
  for (int i=0; i<45; i++) {
    moveServos(90, 75-i, 162, 141); delay(5); 
  }
}

void readSensor() {
  vsensor1 = analogRead(sensor1);//delay(1);
  vsensor2 = analogRead(sensor2);//delay(1);
  vsensor3 = analogRead(sensor3);//delay(1);
  vsensor4 = analogRead(sensor4);

  // 100,964  160,1000 921,150 0, 660
  vsensor1 = map(vsensor1, 50, 964, 0,180);
  vsensor2 = map(vsensor2, 160, 1000, 0,180);
  vsensor3 = map(vsensor3, 1000, 0, 0,180);
  vsensor4 = map(vsensor4, 0, 1023, 127,176);

  moveServos(180-vsensor1, 180-vsensor2, vsensor3, vsensor4);

  recording(180-vsensor1, 180-vsensor2, vsensor3, vsensor4);
//  Serial.print(vsensor1);Serial.print(',');
//  Serial.print(vsensor2);Serial.print(',');
//  Serial.print(vsensor3);Serial.print(',');
//  Serial.println(vsensor4);
}

void moveServos(int p1, int p2, int p3, int p4) {
  if (p1 >= 0 && p1 <= 180) {    
    myservo1.write(p1);
  }
  if (p2 >= 0 && p2 <= 180) myservo2.write(p2);
  if (p3 > 0 && p3 <= 180) myservo3.write(p3);
  if (p4 > 25) myservo4.write(p4);
}

int serialListener() {
  if (com1.available()) {
    char channel[10];
    int i = 0;
    while (com1.available()) {
      char ch = com1.read(); delay(5);
      if (ch != '\n' && i < 4) {
        if (i < 4) {
          channel[i] = ch;
          i++;
          channel[i] = '\0';
        }
      } else {
        pos1 = ((byte) channel[0]) - 30;
        pos2 = ((byte) channel[1]) - 30;
        pos3 = ((byte) channel[2]) - 30;
        pos4 = ((byte) channel[3]) - 30;

//        com1.print(pos1);com1.print(',');
//        com1.print(pos2);com1.print(',');
//        com1.print(pos3);com1.print(',');
//        com1.println(pos4);
        
        if (pos1==200 && pos2==201) {
          tepukTangan(pos3-200);
        } else if (pos1==200 && pos2==202) {
          beriHormat();
        } else {        
          moveServos(pos1, pos2, pos3, pos4);
        }
        channel[0] = '\0';
      }
    }
    return 1;
  } else return 0;
}

void loop() {
  readSW();
  readSensor();

  if (play_active==1) {
    playing();
  }
}