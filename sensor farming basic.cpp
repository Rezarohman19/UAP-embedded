#include <DHT.h>           // Library untuk sensor DHT22
#include <Wire.h>          // Diperlukan untuk komunikasi I2C (untuk LCD)
#include <LiquidCrystal_I2C.h> // Library untuk LCD I2C

// Konfigurasi Sensor DHT22
#define DHTPIN 2           // Pin data DHT22 terhubung ke pin Digital 2 Arduino
#define DHTTYPE DHT11      // Tipe sensor adalah DHT22
DHT dht(DHTPIN, DHTTYPE);  // Membuat objek DHT

// Konfigurasi LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD: alamat I2C, 16 kolom, 2 baris

// Konfigurasi LDR dan LED
#define LDR_PIN A0         // Pin analog tempat LDR terhubung
#define LED_PIN 7          // Pin digital tempat LED terhubung

int ldrValue = 0;          // Variabel untuk menyimpan nilai pembacaan LDR
int threshold = 200;       // Nilai ambang batas LDR (sesuaikan ini berdasarkan kondisi cahaya Anda)

void setup() {
  dht.begin();                       // Memulai sensor DHT
  pinMode(LED_PIN, OUTPUT);         // Mengatur pin LED sebagai OUTPUT
  Serial.begin(9600);               // Memulai komunikasi serial untuk debugging

  lcd.init();                       // Inisialisasi LCD
  delay(1000);                      // Delay tambahan untuk kestabilan LCD
  lcd.backlight();                  // Menyalakan backlight

  // Tes awal tampilkan pesan
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Memulai Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Harap Tunggu...");
  delay(2000);
  lcd.clear();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  ldrValue = analogRead(LDR_PIN);

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  if (ldrValue > threshold) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON (Gelap)");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF (Terang)");
  }

  lcd.clear(); // Bersihkan layar setiap loop (agar tidak menimpa tulisan sebelumnya)

  if (isnan(humidity) || isnan(temperature)) {
    lcd.setCursor(0, 0);
    lcd.print("DHT Gagal");
    lcd.setCursor(0, 1);
    lcd.print("Cek Koneksi!");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Suhu: ");
    lcd.print(temperature, 1);
    lcd.print((char)223);  // Karakter derajat °
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Lembab: ");
    lcd.print(humidity, 0);
    lcd.print("%");
  }

  delay(2000); // Jeda antar tampilan (bisa diubah)
}
