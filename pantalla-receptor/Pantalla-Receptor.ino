#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "BluetoothSerial.h"



#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

String valores;
float X1,Y1,Z1,X2,Y2,Z2,X3,Y3,Z3,X4,Y4,Z4,X5,Y5,Z5;

///////////////////////////////////////////////////////
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
//#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1

#define CLK_PIN 18
#define DATA_PIN 23
#define CS_PIN 5

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

///////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);

  SerialBT.begin("ESP32test"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with Bluetooth!");

  P.begin();
  P.displayClear();
  P.displaySuspend(false);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

}

void loop() {
  if (SerialBT.available()) {
    String data = SerialBT.readStringUntil('\n'); // Lee la cadena hasta encontrar un salto de línea

    if (data.startsWith("D")) {
      // Comienza con 'D', que indica que es una cadena de datos de sensores
      data.remove(0, 1); // Elimina el carácter 'D' inicial
      char datos[100]; // Reemplaza 100 con el tamaño máximo esperado para tus datos
      data.toCharArray(datos, 100); // Convierte la cadena a un array de caracteres
      // Tokenizar la cadena
      char *token = strtok(datos, ",");
      int index = 0;

      // Iterar a través de los tokens
      while (token != NULL && index < 15) {
        // Asignar el valor del token a la variable correspondiente
        switch (index) {
          case 0: X1 = atof(token); break;
          case 1: Y1= atof(token); break;
          case 2: Z1 = atof(token); break;
          case 3: X2 = atof(token); break;
          case 4: Y2 = atof(token); break;
          case 5: Z2 = atof(token); break;
          case 6: X3 = atof(token); break;
          case 7: Y3 = atof(token); break;
          case 8: Z3 = atof(token); break;
          case 9: X4 = atof(token); break;
          case 10: Y4 = atof(token); break;
          case 11: Z4 = atof(token); break;
          case 12: X5 = atof(token); break;
          case 13: Y5 = atof(token); break;
          case 14: Z5 = atof(token); break;
        }
        index++;

        // Obtener el próximo token
        token = strtok(NULL, ",");
      }

      // Mostrar los valores en el puerto serial (solo para verificar)
      Serial.print("x1: "); Serial.print(X1, 1);
      Serial.print(",y1: "); Serial.print(Y1, 1);
      Serial.print(",z1: "); Serial.print(Z1, 1);
      Serial.print(",x2: "); Serial.print(X2, 1);
      Serial.print(",y2: "); Serial.print(Y2, 1);
      Serial.print(",z2: "); Serial.print(Z2, 1);
      Serial.print(",x3: "); Serial.print(X3, 1);
      Serial.print(",y3: "); Serial.print(Y3, 1);
      Serial.print(",z3: "); Serial.print(Z3, 1);
      Serial.print(",x4: "); Serial.print(X4, 1);
      Serial.print(",y4: "); Serial.print(Y4, 1);
      Serial.print(",z4: "); Serial.print(Z4, 1);
      Serial.print(",x5: "); Serial.print(X5, 1);
      Serial.print(",y5: "); Serial.print(Y5, 1);
      Serial.print(",z5: "); Serial.println(Z5, 1);

      // Pausa para evitar un procesamiento continuo (puedes ajustar esto según tus necesidades)
      delay(10);

      if(X1 >= -80 && X1 <= -40 && Y1 <= 30 && Y1 >= 0 && Z1 >= -15 && Z1 <= 25 && X2 >= -87 && X2 <= -40 && X3 >= -95 && X3 <= -44 && X4 >= -91 && X4 <= -49){
        Serial.println("A");
        digitalWrite(CS_PIN, LOW);
        P.displayText("A", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();
        delay(300); 
        digitalWrite(CS_PIN, HIGH);
      }
      else if( X1 <= 70 && X1 >= 25 && Y1 >= -35 && Y1 <= 10 && Z1 >= -10 && Z1 <= 30 && X2 <= 82 && X2 >= 34 && X3 <= 86 && X3 >= 41){
        Serial.println("B");
        digitalWrite(CS_PIN, LOW);
        P.displayText("B", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();
        delay(300); 
        digitalWrite(CS_PIN, HIGH);
      }
      else if (X1 <= 20 && X1 >= -20 && Y1 >= 2 && Y1 <= 47 && Z1 >= 3 && Z1 <= 46 &&
         X2 <= 26 && X2 >= -17 && Y2 >= 5 && Y2 <= 54 && Z2 >= -20 && Z2 <= 24 &&
         X3 <= 11 && X3 >= -29 && Y3 >= 4 && Y3 <= 53 && Z3 >= -3 && Z3 <= 29 &&
         Y4 >= -24 && Z4 >= -20 &&
         X5 <= 57 && X5 >= -15 && Y5 >= -55 && Y5 <= 17 && Z5 >= -20 && Z5 <= 20){
        Serial.println("C");
        digitalWrite(CS_PIN, LOW);
        P.displayText("C", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();
        delay(300); 
        digitalWrite(CS_PIN, HIGH);
      }
      else if(X1 <= -6 && X1 >= -35 && Y1 >= 13 && Y1 <= 62 && Z1 >= 13 && Z1 <= 46 &&
         X2 <= -10 && X2 >= -40 && Y2 >= 21 && Y2 <= 69){
        Serial.println("D");
        digitalWrite(CS_PIN, LOW);
        P.displayText("D", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();
        delay(300); 
        digitalWrite(CS_PIN, HIGH);
      }

      else if (X1 <= 10 && X1 >= -54 && Y1 >= 6 && Y1 <= 47 &&
         X2 <= -10 && X2 >= -52 && Y2 >= 29 && Y2 <= 70 &&
         X3 <= 46 && X3 >= -80 && Y3 >= -6 && Y3 <= 35 &&
         X4 <= 64 && X4 >= -42 && Y4 >= -42 && Y4 <= 20 &&
         X5 <= 34 && X5 >= -8 && Y5 >= -20 && Y5 <= 20) {

        Serial.println("E");
        digitalWrite(CS_PIN, LOW);
        P.displayText("E", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();
        delay(300); 
        digitalWrite(CS_PIN, HIGH);
    
      }

      else if (X1 <= -23.8 && X1 >= -63.8 && Y1 >= 0.8 && Y1 <= 40.8 &&
         X2 <= -23.0 && X2 >= -63.0 && Y2 >= 2.2 && Y2 <= 42.2 &&
         X3 >= 12.2 && X3 <= 52.2 && Y3 >= 21.3 && Y3 <= 61.3 &&
         X4 >= 20.8 && X4 <= 60.8 && Y4 >= -15.9 && Y4 <= 24.1 &&
         X5 >= 29.2 && X5 <= 69.2 && Y5 >= -28.0 && Y5 <= 12.0) {

        Serial.println("R");
        digitalWrite(CS_PIN, LOW);
        P.displayText("R", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();
        delay(300); 
        digitalWrite(CS_PIN, HIGH);
   
      }

      else{
        Serial.println("- - -");
        digitalWrite(CS_PIN, LOW);
        P.displayText("---", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();
        delay(300); 
        digitalWrite(CS_PIN, HIGH);

      }
    }
  }
  
}

