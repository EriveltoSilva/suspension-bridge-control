/*
**dois semaforos
**três ultrassonicos
**lcd 16x02
**um motor
**botao modo manual
*/

////////////////////////// INCLUSÃO ///////////////////////////////
#include <Wire.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>

#define DEBUG false

#define MAX_DISTANCE_FRONT 100
#define MAX_DISTANCE_BACK 30
#define TIME_TO_CLOSE_BRIDGE 5000

#define led_verde_1 2
#define led_amarelo_1 3
#define led_vermelho_1 4

#define led_verde_2 5
#define led_amarelo_2 6
#define led_vermelho_2 7

#define LIMIT_BRIDGE_OPEN 8
#define LIMIT_BRIDGE_CLOSE 9

#define TRIGGER_TRAS 12
#define ECHO_TRAS 13

#define TRIGGER_FRENTE 10
#define ECHO_FRENTE 11

#define MOTOR_BRIDGE_OPEN A1
#define MOTOR_BRIDGE_CLOSE A0
#define BUZZER A2
#define BUTTON_CHANGE_MODE_OPEN_CLOSE A3

#define MARGEM 25
#define VALUE_CHANGE_MODE_MIN 200
#define VALUE_CHANGE_MODE_MAX 512
#define VALUE_OPEN_CLOSE_BRIDGE 900


// 5V ---- > 1024
// 1.8V ---- X
// x= (1.8v*1024)/5v
// x = 368.64

//Recursos
char mode = 'A';
char modeAnt = 'A';

bool flagBarco = false;
bool flagAlert = false;
bool flagAbrindo = false;
bool flagPassando = false;
bool flagFechando = false;

bool flagChangeMode = false;

char status_semaforo = 'R';

int distanciaFrente = 0;
int distanciaTras = 0;
String distanciaBarco = "";
String estadoSemaforo = "";
unsigned long int tempoDelay = 0, tempoBarco = 0;

//********************************  *************************************
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Set the LCD address to 0x27 for a 16x02
NewPing ultraFrente(TRIGGER_FRENTE, ECHO_FRENTE, MAX_DISTANCE_FRONT);
NewPing ultraTras(TRIGGER_TRAS, ECHO_TRAS, MAX_DISTANCE_BACK);

void modeRed();
void modeGreen();
void modeYellow();
void modeOff();
void lerSensores();
void turnOnAlarm();
void turnOffAlarm();

void setup() {
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);

  pinMode(MOTOR_BRIDGE_CLOSE, OUTPUT);
  digitalWrite(MOTOR_BRIDGE_CLOSE, LOW);

  pinMode(MOTOR_BRIDGE_OPEN, OUTPUT);
  digitalWrite(MOTOR_BRIDGE_OPEN, LOW);

  pinMode(LIMIT_BRIDGE_OPEN, INPUT_PULLUP);
  pinMode(LIMIT_BRIDGE_CLOSE, INPUT_PULLUP);


  //Semaforo 1
  pinMode(led_verde_1, OUTPUT);
  pinMode(led_amarelo_1, OUTPUT);
  pinMode(led_vermelho_1, OUTPUT);
  digitalWrite(led_verde_1, HIGH);
  digitalWrite(led_amarelo_1, LOW);
  digitalWrite(led_vermelho_1, LOW);

  //Semaforo 2
  pinMode(led_verde_2, OUTPUT);
  pinMode(led_amarelo_2, OUTPUT);
  pinMode(led_vermelho_2, OUTPUT);
  digitalWrite(led_verde_2, HIGH);
  digitalWrite(led_amarelo_2, LOW);
  digitalWrite(led_vermelho_2, LOW);
  delay(200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("##### ISPB #####");
  lcd.setCursor(0, 1);
  lcd.print(" PROJECTO FINAL");
  Serial.begin(9600);
  delay(2500);
  digitalWrite(BUZZER, LOW);
  if (isBridgeClose())
    modeGreen();
  else
    modeRed();
}


void loop() {
  receiveData();
  verifyMode();
  verifyClosing();
  readButton();

  if (millis() - tempoDelay > 500) {
    tempoDelay = millis();
    lerSensores();
    printDataLCD(lcd);
    sendData();
  }

  if (mode == 'A') {

    if (flagAbrindo && flagBarco) {
      if (isBridgeClose()) {
        modeRed();
        testOpenBridge();
        turnOnAlarm();
        Serial.println("Abrindo, ha barco");
        printManualLCD(lcd, "     PONTE      ", "    ABRINDO     ");
        while (!isBridgeOpen()) {
          proccessingSending();
        }
        stopBridge();
        flagAbrindo = false;
        Serial.println("## ponte aberta ##");
        printManualLCD(lcd, "     PONTE      ", "### ABERTA ###");
        turnOffAlarm();
      }
    }
  }

  if ((millis() - tempoBarco > TIME_TO_CLOSE_BRIDGE) && flagPassando) {
    flagBarco = false;
    flagPassando = false;
    flagFechando = true;
    Serial.println("--> PONTE FECHANDO");
    if (mode == 'A') {
      modeRed();
      testCloseBridge();
      turnOnAlarm();
      Serial.println("FECHANDO...");
      printManualLCD(lcd, "     PONTE      ", "   FECHANDO     ");
      while (!isBridgeClose())
        ;
      stopBridge();
      flagBarco = false;
      flagPassando = false;
      modeGreen();
      printManualLCD(lcd, "     PONTE      ", "### FECHADA ###");
      turnOffAlarm();
      delay(2000);
    }
  }
  delay(10);
}



void lerSensores() {
  distanciaFrente = ultraFrente.ping_cm();
  distanciaTras = ultraTras.ping_cm();

  if ((distanciaFrente > 30) && mode == 'A') {
    flagBarco = true;
    flagAlert = true;
    distanciaBarco = "NAVIO Á:" + String(distanciaFrente);
    modeAlert();
  } else if ((distanciaFrente > 3) && mode == 'A') {
    flagBarco = true;
    flagAlert = false;
    distanciaBarco = "NAVIO Á:" + String(distanciaFrente);
    flagAbrindo = true;
  } else if ((distanciaFrente < 2) && flagAlert && mode == 'A') {
    if (flagAbrindo)
      modeRed();
    else if (!flagAbrindo && isBridgeClose()) {
      flagAlert = false;
      flagBarco = false;
      modeGreen();
    }
  }

  //if (distanciaTras > 2 && flagBarco && mode == 'A') {
  if (distanciaTras > 2 && isBridgeOpen() && mode == 'A') {
    Serial.println("NAVIO PASSANDO  ");
    distanciaBarco = "NAVIO PASSANDO  ";
    modeRed();
    flagPassando = true;
    tempoBarco = millis();
  }

  if(DEBUG)
  {
  Serial.println("LIMIT_CLOSE:" + String(digitalRead(LIMIT_BRIDGE_CLOSE)));
  Serial.println("LIMIT_OPEN:" + String(digitalRead(LIMIT_BRIDGE_OPEN)));
  Serial.println("DistanciaFrente:" + String(distanciaFrente));
  Serial.println("DistanciaTras:" + String(distanciaTras));
  Serial.println("----------------------------------------");
  }
}


//////////////////////////////////////////////////////////
void printDataLCD(LiquidCrystal_I2C lcd) {
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  if (flagBarco) {
    lcd.print("NAVIO DETECTADO");
    lcd.setCursor(0, 1);
    lcd.print(distanciaBarco);
    lcd.print("cm");
  } else {
    lcd.print("#   SEMAFORO   #");
    lcd.setCursor(0, 1);
    lcd.print(estadoSemaforo);
  }

  lcd.setCursor(15, 1);
  lcd.print(mode);
}


void printManualLCD(LiquidCrystal_I2C lcd, String linha1, String linha2) {
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linha1);
  lcd.setCursor(0, 1);
  lcd.print(linha2);
}

bool isBridgeClose() {
  return (!digitalRead(LIMIT_BRIDGE_CLOSE));
}

bool isBridgeOpen() {
  return (!digitalRead(LIMIT_BRIDGE_OPEN));
}


void openBridge() {
  if (!isBridgeClose())
    digitalWrite(MOTOR_BRIDGE_CLOSE, HIGH);
  else {
    Serial.println("JÁ ESTÁ FECHADA!");
    stopBridge();
  }
}

void closeBridge() {                        //
  if (!isBridgeOpen())                      // está fechado
    digitalWrite(MOTOR_BRIDGE_OPEN, HIGH);  // abrir
  else {
    Serial.println("JÁ ESTÁ ABERTA!");
    stopBridge();
  }
}


void testOpenBridge() {
  if (!isBridgeOpen())
    digitalWrite(MOTOR_BRIDGE_OPEN, HIGH);
  else {
    Serial.println("JÁ ESTÁ ABERTA!");
    stopBridge();
  }
}

void testCloseBridge() {                     //
  if (!isBridgeClose())                      // está aberto
    digitalWrite(MOTOR_BRIDGE_CLOSE, HIGH);  // fechado
  else {
    Serial.println("JÁ ESTÁ FECHADO!");  //
    stopBridge();
  }
}

bool isOpenBridgePress() {
  return analogRead(BUTTON_CHANGE_MODE_OPEN_CLOSE) > (VALUE_OPEN_CLOSE_BRIDGE);
}
void proccessingSending()
{
  if (millis() - tempoDelay > 500) {
    tempoDelay = millis();
    sendData();
  }
}

void readButton() {
  if (mode == 'M') {
    if (isOpenBridgePress()) {
      if (isBridgeOpen()) {  // está aberta
        modeRed();
        testCloseBridge();
        turnOnAlarm();
        Serial.println("FECHANDO...");
        printManualLCD(lcd, "     PONTE      ", "   FECHANDO     ");
        while (isOpenBridgePress() && !isBridgeClose())
        {
          proccessingSending();
        }
          
        stopBridge();
        flagBarco = false;
        flagPassando = false;
        modeGreen();
        Serial.println("Solte o botão");
        printManualLCD(lcd, "     PONTE      ", "### FECHADA ###");
        while (isOpenBridgePress())
          ;
        turnOffAlarm();
        delay(2000);
      } else if (isBridgeClose()) {
        modeRed();
        testOpenBridge();
        turnOnAlarm();
        Serial.println("ABRINDO...");
        printManualLCD(lcd, "     PONTE      ", "    ABRINDO     ");
        while (isOpenBridgePress() && !isBridgeOpen())
        {
          proccessingSending();
        }
        stopBridge();
        Serial.println("Solte o botão");
        printManualLCD(lcd, "     PONTE      ", "### ABERTA ###");
        while (isOpenBridgePress())
          ;
        turnOffAlarm();
      } else  //no meio if(!isBridgeClose() && !isBridgeOpen())
      {
        modeRed();
        testCloseBridge();
        turnOnAlarm();
        Serial.println("FECHANDO NO MEIO...");
        printManualLCD(lcd, "     PONTE      ", "   FECHANDO     ");
        while (isOpenBridgePress() && !isBridgeClose())
        {
          proccessingSending();
        }
        stopBridge();
        flagBarco = false;
        flagPassando = false;
        modeGreen();
        Serial.println("Solte o botão");
        printManualLCD(lcd, "     PONTE      ", "### FECHADA ###");
        while (isOpenBridgePress())
          ;
        turnOffAlarm();
        delay(2000);
      }
    }
  }
}

void stopBridge() {
  digitalWrite(MOTOR_BRIDGE_OPEN, LOW);
  digitalWrite(MOTOR_BRIDGE_CLOSE, LOW);
}

void bip() {
  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(BUZZER, LOW);
}

void turnOnAlarm() {
  digitalWrite(BUZZER, HIGH);
}

void turnOffAlarm() {
  digitalWrite(BUZZER, LOW);
}


void verifyClosing() {
  if (isBridgeOpen())
    modeRed();
  if (flagFechando && !digitalRead(LIMIT_BRIDGE_CLOSE)) {
    flagFechando = false;
    stopBridge();
  }
}

void verifyMode() {
  int valorAnalogico = analogRead(BUTTON_CHANGE_MODE_OPEN_CLOSE);

  if (valorAnalogico < VALUE_CHANGE_MODE_MIN || valorAnalogico > VALUE_CHANGE_MODE_MAX || flagChangeMode) {
    flagChangeMode = false;
    return;
  }

  flagChangeMode = true;
  mode = (mode == 'M') ? 'A' : 'M';

  if (mode != modeAnt) {
    modeAnt = mode;
    flagAbrindo = false;
    flagBarco = false;
    flagPassando = false;
    flagFechando = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("###   MODO    ###");
    lcd.setCursor(0, 1);
    if (mode == 'A')
      lcd.print("  AUTOMATICO   ");
    else
      lcd.print("     MANUAL     ");
    bip();
    delay(2500);
  }
}


///////////////////////////////////////////////////////////
void modeAlert() {
  static bool aux = false;
  if (aux) {
    modeYellow();
    turnOnAlarm();
  } else {
    modeOff();
    turnOffAlarm();
  }
  status_semaforo = 'A';
  aux = !aux;
}

///////////////////////////////////////////////////////////
void modeGreen() {
  digitalWrite(led_verde_1, HIGH);
  digitalWrite(led_amarelo_1, LOW);
  digitalWrite(led_vermelho_1, LOW);

  digitalWrite(led_verde_2, HIGH);
  digitalWrite(led_amarelo_2, LOW);
  digitalWrite(led_vermelho_2, LOW);
  estadoSemaforo = "#### VERDE ####";
  status_semaforo = 'G';
  turnOffAlarm();
}

///////////////////////////////////////////////////////////
void modeYellow() {
  digitalWrite(led_verde_1, LOW);
  digitalWrite(led_amarelo_1, HIGH);
  digitalWrite(led_vermelho_1, LOW);

  digitalWrite(led_verde_2, LOW);
  digitalWrite(led_amarelo_2, HIGH);
  digitalWrite(led_vermelho_2, LOW);
  status_semaforo = 'Y';
  estadoSemaforo = "### AMARELO ###";
}

///////////////////////////////////////////////////////////
void modeRed() {
  digitalWrite(led_verde_1, LOW);
  digitalWrite(led_amarelo_1, LOW);
  digitalWrite(led_vermelho_1, HIGH);

  digitalWrite(led_verde_2, LOW);
  digitalWrite(led_amarelo_2, LOW);
  digitalWrite(led_vermelho_2, HIGH);
  estadoSemaforo = "## VERMELHO ##";
  status_semaforo = 'R';
  turnOnAlarm();
}

///////////////////////////////////////////////////////////
void modeOff() {
  digitalWrite(led_verde_1, LOW);
  digitalWrite(led_amarelo_1, LOW);
  digitalWrite(led_vermelho_1, LOW);

  digitalWrite(led_verde_2, LOW);
  digitalWrite(led_amarelo_2, LOW);
  digitalWrite(led_vermelho_2, LOW);
}


void sendData() {
  // status_semaforo: A-alarme, Y-yellow, R-red, G-green,
  // status_bridge: C-close, O-open,
  //D*mode*status_semaforo*status_bridge*

  char status_bridge = isBridgeClose()? 'C' : 'O';
  bool closing = (flagAbrindo && flagBarco);
  Serial.println("D*" + String(mode) + "*" + String(status_semaforo) + "*" + String(status_bridge) + "*"+String(distanciaFrente)+"*"+String(closing)+"*");
}

void receiveData() {
  if (Serial.available()) {
    while (Serial.available()) {
      char rx = (char)Serial.read();
      switch (rx) {
        case 'B':processCommand();break;
        case 'M': mode = (mode=='M')?'A':'M'; break;
      }
    }
  }
}

void processCommand() {
  if (mode == 'M') {
    if (isBridgeOpen()) {  // está aberta
      modeRed();
      testCloseBridge();
      turnOnAlarm();
      Serial.println("FECHANDO...");
      printManualLCD(lcd, "     PONTE      ", "   FECHANDO     ");
      while (!isBridgeClose())
        ;
      stopBridge();
      flagBarco = false;
      flagPassando = false;
      modeGreen();
      printManualLCD(lcd, "     PONTE      ", "### FECHADA ###");
      turnOffAlarm();
      delay(2000);
    } else if (isBridgeClose()) {
      modeRed();
      testOpenBridge();
      turnOnAlarm();
      Serial.println("ABRINDO...");
      printManualLCD(lcd, "     PONTE      ", "    ABRINDO     ");
      while (!isBridgeOpen())
        ;
      stopBridge();
      Serial.println("Solte o botão");
      printManualLCD(lcd, "     PONTE      ", "### ABERTA ###");
      turnOffAlarm();
    } else {  //no meio if(!isBridgeClose() && !isBridgeOpen())
      modeRed();
      testCloseBridge();
      turnOnAlarm();
      Serial.println("FECHANDO NO MEIO...");
      printManualLCD(lcd, "     PONTE      ", "   FECHANDO     ");
      while (!isBridgeClose())
        ;
      stopBridge();
      flagBarco = false;
      flagPassando = false;
      modeGreen();
      Serial.println("Solte o botão");
      printManualLCD(lcd, "     PONTE      ", "### FECHADA ###");
      turnOffAlarm();
      delay(2000);
    }
  }
}
