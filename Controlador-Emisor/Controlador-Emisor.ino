#include <Wire.h>
#include <math.h>
#include "BluetoothSerial.h"


#define MPU 0x68
#define A_R 16384.0
#define G_R 131.0
#define RAD_A_DEG 57.295779

#define ledR 25
#define ledB 32
#define ledG 33

BluetoothSerial SerialBT;

String MACadd = "FC:F5:C4:2F:57:6E";//Write Glove Recriver side MAC address
uint8_t address[6]  = {0xFC, 0xF5, 0xC4, 0x2F, 0x57, 0x6E};//Write Drone side MAC address in HEX
bool connected;

int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
String valores;

const int pines[] = {13, 12, 14, 27, 26};
const int numPines = sizeof(pines) / sizeof(pines[0]);

float Acc[numPines][2];
float Gy[numPines][3];
float Angle[numPines][3];

long tiempo_prev;
float dt;

void setup() {
  Serial.begin(115200);

  
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

   digitalWrite(ledR, HIGH);
  digitalWrite(ledG, LOW);
  digitalWrite(ledB, LOW);

  for (int i = 0; i < numPines; i++) {
    pinMode(pines[i], OUTPUT);
  }

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  delay(20);

  SerialBT.begin("ESP32test", true); 
  Serial.println("The device started in master mode, make sure remote BT device is on!");
  
  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  connected = SerialBT.connect(address);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  // disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();
}




void loop() {

 float sensorAngles[numPines * 3];
  
  Serial.print("90,");
  for (int i = 0; i < numPines; i++) {
    digitalWritePins(i);
    leerSensores(i);
    sensorAngles[i * 3] = Angle[i][0];
    sensorAngles[i * 3 + 1] = Angle[i][1];
    sensorAngles[i * 3 + 2] = Angle[i][2];

  }

   sendAnglesOverBluetooth(sensorAngles); 

   Serial.print(",-90");
   Serial.println();
   digitalWrite(ledR, LOW);
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, LOW);  


}

void sendAnglesOverBluetooth(float sensorAngles[]) {
  String dataString = "D"; // Start the data string with a identifier


  for (int i = 0; i < numPines; i++) {
    dataString += String(sensorAngles[i * 3], 1) + ",";     // Angle[0]
    dataString += String(sensorAngles[i * 3 + 1], 1) + ","; // Angle[1]
    dataString += String(sensorAngles[i * 3 + 2], 1);        // Angle[2]

    if (i < numPines - 1) {
      dataString += ","; // Añade una coma entre los valores
    }
  }

  Serial.print(dataString);       // Imprime la cadena de datos en Serial para verificación
  SerialBT.println(dataString);
}

void digitalWritePins(int activePin) {
  for (int i = 0; i < numPines; i++) {
    digitalWrite(pines[i], i == activePin ? LOW : HIGH);
  }
}

void leerSensores(int index) {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

  Acc[index][1] = atan(-1 * (AcX / A_R) / sqrt(pow((AcY / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_A_DEG;
  Acc[index][0] = atan((AcY / A_R) / sqrt(pow((AcX / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_A_DEG;

  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

  Gy[index][0] = GyX / G_R;
  Gy[index][1] = GyY / G_R;
  Gy[index][2] = GyZ / G_R;

  dt = (millis() - tiempo_prev) / 1000.0;
  tiempo_prev = millis();


  Angle[index][0] = 0.98 * (Angle[index][0] + Gy[index][0] * dt) + 0.02 * Acc[index][0];
  Angle[index][1] = 0.98 * (Angle[index][1] + Gy[index][1] * dt) + 0.02 * Acc[index][1];
  Angle[index][2] = Angle[index][2] + Gy[index][2] * dt;

  // valores = "x:" + String(Angle[index][0]) + ",y:" + String(Angle[index][1]) + ",z:" + String(Angle[index][2]);

  // // valores = String(Angle[index][0]);
  // Serial.print(valores + "\t");
}
