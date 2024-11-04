#include <DHT.h>        // Biblioteca do sensor DHT22
#include <WiFi.h>       // Biblioteca para comunicação Wi-Fi
#include <ThingSpeak.h> // Biblioteca para envio de dados para o ThingSpeak
#include <time.h>       // Biblioteca para obter o tempo atual via NTP
#include <ESP32Servo.h> // Biblioteca para controle de servo no ESP32

#define DHT_PIN 13      // Define o pino para o sensor DHT22
#define SERVO_PIN 18    // Define o pino para o servo motor

DHT dht(DHT_PIN, DHT22);
Servo servo;

#define ssid  "Wokwi-GUEST"
#define senha ""

#define myChannelNumber 2723410       
#define myApiKey  "N8CXEDO8JWY5G1GS"  

WiFiClient client;

int status;

void setup() {
  dht.begin();          
  servo.attach(SERVO_PIN); 
  servo.write(0);

  Serial.begin(115200);
  delay(100);
  Serial.print("Se conectando a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, senha);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  WiFi.mode(WIFI_STA);            
  ThingSpeak.begin(client);

  configTime(0, 0, "pool.ntp.org");
  delay(1000);
}

void loop() {
  dht.read(); 
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  ThingSpeak.setField(1, temperature);  
  ThingSpeak.setField(2, humidity);     
  status = ThingSpeak.writeFields(myChannelNumber, myApiKey);

  Serial.println("Temperature: " + String(temperature, 2) + "°C");
  Serial.println("Humidity: " + String(humidity, 1) + "%");

  if (status == 200) {
    Serial.println("Dados enviados com sucesso");
  } else {
    Serial.println("Erro de envio " + String(status));
  }
  
  if (humidity < 50.0 || temperature > 40.0) {
    Serial.println("Umidade baixa! Ativando irrigador...");
    servo.write(90);  // Posiciona o servo em 90 graus para ativar o irrigador
  } else {
    Serial.println("Umidade adequada. Irrigador desligado.");
    servo.write(0);   // Retorna o servo para a posição inicial
  }

  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%H:%M:%S %d/%m/%Y", timeinfo);

  Serial.print("Hora de aceitação: ");
  Serial.println(buffer);

  Serial.println("---");

  delay(10000);
}
