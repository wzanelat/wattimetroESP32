#include <EmonLib.h> //INCLUSÃO DE BIBLIOTECA
#include <Wire.h> //INCLUSÃO DE BIBLIOTECA
#include <Adafruit_GFX.h> //INCLUSÃO DE BIBLIOTECA
#include <Adafruit_SSD1306.h> //INCLUSÃO DE BIBLIOTECA

#define VOLT_CAL 114.7 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO)
#define CURRENT_CAL 16.80 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)


const int pinoSensor = 34; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
float ruido = 0.08; //RUÍDO PRODUZIDO NA SAÍDA DO SENSOR (DEVE SER AJUSTADO COM A CARGA DESLIGADA APÓS CARREGAMENTO DO CÓDIGO NO ARDUINO)

Adafruit_SSD1306 display = Adafruit_SSD1306(); //OBJETO DO TIPO Adafruit_SSD1306
EnergyMonitor emon1; //CRIA UMA INSTÂNCIA
EnergyMonitor emon2; //CRIA UMA INSTÂNCIA

void setup(){  
  pinMode(32, INPUT);
  Serial.begin(9600); //INICIALIZA A SERIAL
  Wire.begin(); //INICIALIZA A BIBLIOTECA WIRE
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //INICIALIZA O DISPLAY COM ENDEREÇO I2C 0x3C
  display.setTextColor(WHITE); //DEFINE A COR DO TEXTO
  display.setTextSize(2); //DEFINE O TAMANHO DA FONTE DO TEXTO
  display.clearDisplay(); //LIMPA AS INFORMAÇÕES DO DISPLAY
  emon1.current(pinoSensor, CURRENT_CAL); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO)
  emon2.voltage(36, VOLT_CAL, 1.7); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO / MUDANÇA DE FASE)
  int pinStatus = LOW;
}

void loop(){

  emon1.calcVI(17,150); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  float currentDraw = (emon1.Irms)/10; //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO
  currentDraw = currentDraw-ruido; //VARIÁVEL RECEBE O VALOR RESULTANTE DA CORRENTE RMS MENOS O RUÍDO
  
  emon2.calcVI(17,150); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS, TEMPO LIMITE PARA FAZER A MEDIÇÃO)    
  float supplyVoltage   = emon2.Vrms; //VARIÁVEL RECEBE O VALOR DE TENSÃO RMS OBTIDO
  pinStatus = digitalRead(32);

  if(currentDraw < 0){ //SE O VALOR DA VARIÁVEL FOR MENOR QUE 0, FAZ 
      currentDraw = 0; //VARIÁVEL RECEBE 0
  }


  if(pinStatus == HIGH){
    display.clearDisplay(); //LIMPA AS INFORMAÇÕES DO DISPLAY
    display.setCursor(40,10); //POSIÇÃO EM QUE O CURSOR IRÁ FAZER A ESCRITA
    display.print("Inver."); //ESCREVE O TEXTO NO DISPLAY
    Serial.println("Inver. de Fase"); //IMPRIME O TEXTO NA SERIAL
    delay(200); //INTERVALO DE 1,5 SEGUNDOS
    display.clearDisplay(); //LIMPA AS INFORMAÇÕES DO DISPLAY
  } else{
      float power = currentDraw * supplyVoltage;
      display.clearDisplay(); //LIMPA AS INFORMAÇÕES DO DISPLAY
      display.setCursor(40,10); //POSIÇÃO EM QUE O CURSOR IRÁ FAZER A ESCRITA
      display.print(power); //ESCREVE O TEXTO NO DISPLAY
      display.setCursor(90,10); //POSIÇÃO EM QUE O CURSOR IRÁ FAZER A ESCRITA
      display.print(" W"); //ESCREVE O TEXTO NO DISPLAY
      display.display(); //EFETIVA A ESCRITA NO DISPLAY
      Serial.print("Corrente medida: "); //IMPRIME O TEXTO NA SERIAL
      Serial.print(power); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
      Serial.println("A"); //IMPRIME O TEXTO NA SERIAL
      delay(200); //INTERVALO DE 1,5 SEGUNDOS
      display.clearDisplay(); //LIMPA AS INFORMAÇÕES DO DISPLAY
  }
}
