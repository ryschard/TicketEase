#include <Adafruit_LiquidCrystal.h>

int CONTAGEM = 0;
int CONTAGEM_MAXIMA = 0;
float TARIFA = 10.00;  
float valorArrecadado = 0.0;
float valorPerdido = 0.0;
int capacidadeMaxima = 10;  
const int botaoPin = 2;
const int resetBotaoPin = 4;

long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

Adafruit_LiquidCrystal lcd_1(0);

void setup() {
  pinMode(7, OUTPUT);
  pinMode(3, OUTPUT);
  lcd_1.begin(16, 2);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(botaoPin, INPUT);
  pinMode(resetBotaoPin, INPUT);
}

void loop() {

  if (0.01723 * readUltrasonicDistance(6, 5) < 336) {
    if (CONTAGEM < capacidadeMaxima) {
      CONTAGEM++;
      valorArrecadado += TARIFA;
      digitalWrite(7, HIGH);
      analogWrite(3, 10);
      delay(50);
      digitalWrite(7, LOW);
      digitalWrite(3, LOW);
      Serial.print("ENTRADA DE PESSOA. Novo total: ");
      Serial.println(CONTAGEM);
    } else {
      CONTAGEM_MAXIMA++;
      valorPerdido += TARIFA;
      digitalWrite(7, HIGH);  
      delay(50);
      digitalWrite(7, LOW);
    }
    updateLCD();
    delay(1000);
  }

  if (0.01723 * readUltrasonicDistance(11, 10) < 336) {
    if (CONTAGEM > 0) {
      CONTAGEM--;
      digitalWrite(8, HIGH);
      analogWrite(9, 10);
      delay(50);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      Serial.print("SAÍDA DE PESSOA. Novo total: ");
      Serial.println(CONTAGEM);
    }
    updateLCD();
    delay(1000);
  }

  if (digitalRead(botaoPin) == HIGH) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("FIM DO DIA: ");
    lcd_1.print(CONTAGEM);
    lcd_1.setCursor(0, 1);
    lcd_1.print("TOTAL: R$");
    lcd_1.print(valorArrecadado);
    analyzePerformance();
    delay(1000);
  }

  if (digitalRead(resetBotaoPin) == HIGH) {
    CONTAGEM = 0;
    CONTAGEM_MAXIMA = 0;
    valorArrecadado = 0;
    valorPerdido = 0;
    updateLCD();
    Serial.println("Novo dia!");
    delay(1000);
 
  }

  
}

void updateLCD() {
  lcd_1.setCursor(0, 0);
  lcd_1.print("entrada: ");
  lcd_1.print(CONTAGEM);
  lcd_1.setCursor(0, 1);
  lcd_1.print("valor: R$");
  lcd_1.print(valorArrecadado);

}

void analyzePerformance() {
 lcd_1.clear(); 
  lcd_1.setCursor(0, 0);
  lcd_1.print("MAX CAPAC: ");
  lcd_1.print(CONTAGEM_MAXIMA);
  lcd_1.setCursor(0, 1);
  lcd_1.print("VALOR PERDIDO:");
  lcd_1.print(valorPerdido);
  
  Serial.print(" MAXIMA: ");
  Serial.println(CONTAGEM_MAXIMA);
  Serial.print("PERDIDO:");
  Serial.println(valorPerdido);

  if (CONTAGEM_MAXIMA > 10) {
    Serial.println("Recomendacao: Aumentar o numero de onibus.");
  } else if (valorPerdido > 10) {
    Serial.println("Recomendacao: Aumentar o numero de onibus.");
  } else if (CONTAGEM_MAXIMA < 10 && valorPerdido < 10) {
    Serial.println("Recomendacao: Diminuir o numero de onibus.");
  } else {
    Serial.println("Recomendacao: Manter o numero de onibus.");
  }
}