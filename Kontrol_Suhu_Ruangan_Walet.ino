// masukkan library yang dibutuhkan
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// inisialisasi sensor DHT
#define DHT1 2
#define DHT2 3

#define DHTTYPE DHT11   // DHT 11

// menentukan tipe DHT
DHT dht1(DHT1, DHTTYPE);
DHT dht2(DHT2, DHTTYPE);

// inisialisasi untuk relay selenoid valve
const int valve1 = 5;
const int valve2 = 6;

// inisialisasi pompa
const int pompa = 8;

void setup() {
  // memulai lcd dan sensor dht serta menentukan valve dan pompa sebagai output
  Serial.begin(9600);
  Serial.println(F("DHT11 test!"));
  lcd.init();
  lcd.noCursor();
  lcd.backlight();
  lcd.clear();

  dht1.begin();
  dht2.begin();

  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(pompa, OUTPUT);

}

void loop() {
  // tunda 2 detik untuk pengukuran suhu dan kelembapan
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float kelembapan1 = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  float suhu1 = dht1.readTemperature();

  float kelembapan2 = dht2.readHumidity();
  float suhu2 = dht2.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(kelembapan1) || isnan(suhu1)) {
    Serial.println(F("Failed to read from DHT 1 sensor!"));
  }

  if (isnan(kelembapan2) || isnan(suhu2)) {
    Serial.println(F("Failed to read from DHT 2 sensor!"));
  }

  // suhu dan kelembapan terbaca di serial monitor
  Serial.print(F("Kelembapan 1: "));
  Serial.print(kelembapan1);
  Serial.print(F("%  Suhu 1: "));
  Serial.print(suhu1);
  Serial.println(F("°C "));
  Serial.print(F("Kelembapan 2: "));
  Serial.print(kelembapan2);
  Serial.print(F("%  Suhu 2: "));
  Serial.print(suhu2);
  Serial.println(F("°C "));

  if ((kelembapan1 >= 80 && (kelembapan1 <= 90 || kelembapan1 > 90)) &&
      ((suhu1 >= 26 || suhu1 < 26) && suhu1 <= 29)) {
    digitalWrite(valve1, HIGH); // jika kondisi diatas terpenuhi, valve selenoid tidak menyala
  } else {
    digitalWrite(pompa, LOW); // pompa menyala
    digitalWrite(valve1, LOW);  // jika kondisi diatas tidak terpenuhi, valve selenoid menyala
  }

  if ((kelembapan2 >= 80 && (kelembapan2 <= 90 || kelembapan2 > 90)) &&
      ((suhu2 >= 26 || suhu2 < 26) && suhu2 <= 29)) {
    digitalWrite(valve2, HIGH);
  } else {
    digitalWrite(pompa, LOW); // pompa menyala
    digitalWrite(valve2, LOW);
  }

  if (
    (kelembapan1 >= 80 && (kelembapan1 <= 90 || kelembapan1 > 90)) && ((suhu1 >= 26 || suhu1 < 26) && suhu1 <= 29) &&
    (kelembapan2 >= 80 && (kelembapan2 <= 90 || kelembapan2 > 90)) && ((suhu2 >= 26 || suhu2 < 26) && suhu2 <= 29)
  ) {
    digitalWrite(pompa, HIGH); // pompa off
  }

  // pembacaan pada lcd
  lcd.setCursor(0, 0);
  lcd.print("T1="); lcd.print(suhu1); lcd.print("C");
  lcd.setCursor(11, 0);
  lcd.print("H1="); lcd.print(kelembapan1); lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("T2="); lcd.print(suhu2); lcd.print("C");
  lcd.setCursor(11, 1);
  lcd.print("H2="); lcd.print(kelembapan2); lcd.print("%");

}
