// dodawanie bibliotek
#include <SPI.h>
#include <Ethernet.h>
#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ustawiena mudułu PWM
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


// ustawienia silnika
Stepper MojSilnik(200, A3, A2, A1, A0);
int destination = 0;
int position = 0;
int i = 0;


// ustawienia sieciowe
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 0, 0, 20);  // Adres IP!!!
EthernetServer server(80);

void setup() {
  // definiowanie wyjść
  pinMode(8, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  // nawiazywanie lacznosci arduino z komputerem, tworzenie serwera, nawiązywanie połączenia I2C z modułem PWM
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println("Server address:");
  Serial.println(Ethernet.localIP());
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);  
  delay(10);

  // kalibrowanie serw
  pwm.setPWM(0, 0, 110); // klawiatura
  pwm.setPWM(1, 0, 175);
  pwm.setPWM(2, 0, 70);
  pwm.setPWM(3, 0, 75);
  pwm.setPWM(4, 0, 135);
  pwm.setPWM(5, 0, 80);
  pwm.setPWM(6, 0, 125);
  pwm.setPWM(7, 0, 145);
  pwm.setPWM(8, 0, 150);
  pwm.setPWM(9, 0, 77);
  pwm.setPWM(10, 0, 125);
  pwm.setPWM(11, 0, 135);
  pwm.setPWM(12, 0, 200); // karta
  pwm.setPWM(13, 0, 200);
  pwm.setPWM(14, 0, 200);
  pwm.setPWM(15, 0, 200);


  // kalibrowanie silnika głównego
  while (digitalRead(8) != LOW) {
    if (i < 200) {
      MojSilnik.setSpeed(20);
      MojSilnik.step(-1);
      i++;
    } else {
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      exit(0);
    }
  }
  delay(500);
  MojSilnik.step(22);
  delay(500);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}


void loop() {
  while (digitalRead(8) != LOW) {
  // nawiązywanie połączenia
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    String buffer = "";
     
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        buffer += c;
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
          buffer = "";

        }
        // rozpatrywanie warunkow
        else if (c == '\r') {
          // warunki karuzeli
          
          if (buffer.indexOf("GET /?status") >= 0) {
            if ((buffer.indexOf("GET /?status=1_A") >= 0) || (buffer.indexOf("GET /?status=2_B") >= 0) || (buffer.indexOf("GET /?status=3_C") >= 0) || (buffer.indexOf("GET /?status=4_D") >= 0)) {
              destination = 0;
            }
            if ((buffer.indexOf("GET /?status=1_B") >= 0) || (buffer.indexOf("GET /?status=2_C") >= 0) || (buffer.indexOf("GET /?status=3_D") >= 0) || (buffer.indexOf("GET /?status=4_A") >= 0)) {
              destination = 50;
            }
            if ((buffer.indexOf("GET /?status=1_C") >= 0) || (buffer.indexOf("GET /?status=2_D") >= 0) || (buffer.indexOf("GET /?status=3_A") >= 0) || (buffer.indexOf("GET /?status=4_B") >= 0)) {
              destination = 100;
            }
            if ((buffer.indexOf("GET /?status=1_D") >= 0) || (buffer.indexOf("GET /?status=2_A") >= 0) || (buffer.indexOf("GET /?status=3_B") >= 0) || (buffer.indexOf("GET /?status=4_C") >= 0)) {
              destination = 150;
            }
            MojSilnik.setSpeed(20);
            MojSilnik.step(destination - position);
            position = destination;
            if ((buffer.indexOf("GET /?status=1_A") >= 0) || (buffer.indexOf("GET /?status=1_B") >= 0) || (buffer.indexOf("GET /?status=1_C") >= 0) || (buffer.indexOf("GET /?status=1_D") >= 0)) {
              pwm.setPWM(12, 0, 320);
              delay(1500);
              pwm.setPWM(12, 0, 200);
            }
            if ((buffer.indexOf("GET /?status=2_A") >= 0) || (buffer.indexOf("GET /?status=2_B") >= 0) || (buffer.indexOf("GET /?status=2_C") >= 0) || (buffer.indexOf("GET /?status=2_D") >= 0)) {
              pwm.setPWM(13, 0, 320);
              delay(1500);
              pwm.setPWM(13, 0, 200);
            }
            if ((buffer.indexOf("GET /?status=3_A") >= 0) || (buffer.indexOf("GET /?status=3_B") >= 0) || (buffer.indexOf("GET /?status=3_C") >= 0) || (buffer.indexOf("GET /?status=3_D") >= 0)) {
              pwm.setPWM(14, 0, 320);
              delay(1500);
              pwm.setPWM(14, 0, 200);
            }
            if ((buffer.indexOf("GET /?status=4_A") >= 0) || (buffer.indexOf("GET /?status=4_B") >= 0) || (buffer.indexOf("GET /?status=4_C") >= 0) || (buffer.indexOf("GET /?status=4_D") >= 0)) {
              pwm.setPWM(15, 0, 320);
              delay(1500);
              pwm.setPWM(15, 0, 200);
            }
            digitalWrite(A0, LOW);
            digitalWrite(A1, LOW);
            digitalWrite(A2, LOW);
            digitalWrite(A3, LOW);

          // warunki klawiatury
          } else if (buffer.indexOf("GET /?kl") >= 0) {
            if (buffer.indexOf("GET /?kl=0") >= 0) {
              pwm.setPWM(0, 0, 95);
              delay(500);
              pwm.setPWM(0, 0, 110);
            }
            if (buffer.indexOf("GET /?kl=1") >= 0) {
              pwm.setPWM(1, 0, 190);
              delay(500);
              pwm.setPWM(1, 0, 175);
            }
            if (buffer.indexOf("GET /?kl=2") >= 0) {
              pwm.setPWM(2, 0, 85);
              delay(500);
              pwm.setPWM(2, 0, 70);
            }
            if (buffer.indexOf("GET /?kl=3") >= 0) {
              pwm.setPWM(3, 0, 90);
              delay(500);
              pwm.setPWM(3, 0, 75);
            }
            if (buffer.indexOf("GET /?kl=4") >= 0) {
              pwm.setPWM(4, 0, 150);
              delay(500);
              pwm.setPWM(4, 0, 135);
            }
            if (buffer.indexOf("GET /?kl=5") >= 0) {
              pwm.setPWM(5, 0, 90);
              delay(500);
              pwm.setPWM(5, 0, 80);
            }
            if (buffer.indexOf("GET /?kl=6") >= 0) {
              pwm.setPWM(6, 0, 110);
              delay(500);
              pwm.setPWM(6, 0, 125);
            }
            if (buffer.indexOf("GET /?kl=7") >= 0) {
              pwm.setPWM(7, 0, 130);
              delay(500);
              pwm.setPWM(7, 0, 145);
            }
            if (buffer.indexOf("GET /?kl=8") >= 0) {
              pwm.setPWM(8, 0, 130);
              delay(500);
              pwm.setPWM(8, 0, 150);
            }
            if (buffer.indexOf("GET /?kl=9") >= 0) {
              pwm.setPWM(9, 0, 63);
              delay(500);
              pwm.setPWM(9, 0, 77);
            }
            if (buffer.indexOf("GET /?kl=*") >= 0) {
              pwm.setPWM(10, 0, 92);
              delay(500);
              pwm.setPWM(10, 0, 125);
            }
            if (buffer.indexOf("GET /?kl=%") >= 0) {
              pwm.setPWM(11, 0, 105);
              delay(500);
              pwm.setPWM(11, 0, 135);
            }
            delay(10);
          }
        } else {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();  // Zakończ serwer
  }
  }
  delay(500);
  MojSilnik.step(25);
  position = 0;
  destination = 0;
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  delay(1000);
}
