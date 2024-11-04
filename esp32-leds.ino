#include <WiFi.h>         // Biblioteca para comunicação Wi-Fi
#include <ThingSpeak.h>   // Biblioteca para envio de dados para o ThingSpeak

#define LED_RED   5       // Define a porta do pino do LED vermelho
#define LED_YELLOW 17      // Define a porta do pino do LED verde
#define LED_GREEN  16      // Define a porta do pino do LED azul

#define LED_RED_FIELD   1 // Campo para o status do LED vermelho no ThingSpeak
#define LED_YELLOW_FIELD 2 // Campo para o status do LED verde no ThingSpeak
#define LED_GREEN_FIELD  3 // Campo para o status do LED azul no ThingSpeak

#define ssid  "Wokwi-GUEST" // Nome da rede Wi-Fi
#define senha ""            // Senha da rede Wi-Fi (deixe em branco se não tiver)

                                      
#define myChannelNumber 2723410       // Número do canal no ThingSpeak onde você deseja enviar os dados
#define myApiKey  "A1DS9OEBAOCUX5OV"  // Chave de API para o seu canal no ThingSpeak

#define server "api.thingspeak.com"   // Endereço da API do ThingSpeak

WiFiClient client;

float Temperature = 0;   // Variável para armazenar o status do LED vermelho
float Humidity = 0; // Variável para armazenar o status do LED verde

int status;

void setup()
{
  pinMode(LED_RED, OUTPUT);   // Configuração da direção dos pinos do LED RGB
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
   
  Serial.begin(115200); // Define a taxa de transferência serial (115,2 kbps)

  delay(100);

  Serial.print("Se conectando a: ");
  Serial.println(ssid); // Imprime o nome da rede Wi-Fi

  WiFi.begin(ssid, senha);  // Se conecta ao Wi-Fi com as credenciais definidas

  while (WiFi.status() != WL_CONNECTED) // Verifica se a conexão foi bem-sucedida
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); // Imprime o endereço de IP do ESP32

  ThingSpeak.begin(client); // Inicia a comunicação com o ThingSpeak

  digitalWrite(LED_RED, HIGH);   // Inicializa o LED vermelho como desligado
  digitalWrite(LED_GREEN, HIGH); // Inicializa o LED verde como desligado
  digitalWrite(LED_YELLOW, HIGH);  // Inicializa o LED azul como desligado
}

void loop()
{
    Temperature = ThingSpeak.readFloatField(myChannelNumber, 1, myApiKey);  // Lê o dado de temperatura no campo 1
    Humidity = ThingSpeak.readFloatField(myChannelNumber, 2, myApiKey);     // Lê o dado de umidade no campo 2
    
    Serial.println(Temperature);  // Exibe a temperatura
    Serial.println(Humidity); 

    if((Temperature >= -10.1 && Temperature <= 50  ) && Humidity <= 30)
    {
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, LOW); 
        digitalWrite(LED_RED, HIGH);      // Liga o LED vermelho
        Serial.println("LED_RED ligado!");
        Serial.println("Solo Muito Seco!");
        Serial.println("===================");// Imprime "LED vermelho ligado!"
    }
    else if((Temperature >= -10.1 && Temperature <= 50  ) && Humidity >= 70.1 )
    {
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, LOW);     
        digitalWrite(LED_RED, HIGH);      // Liga o LED vermelho
        Serial.println("LED_RED ligado!");
        Serial.println("Solo Muito Humido!");
        Serial.println("==================="); // Imprime "LED vermelho desligado!"
    }
    else if((Temperature >= -10.1 && Temperature <= 50  ) && (Humidity >= 30.1 && Humidity <= 50))
    {
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);     
        digitalWrite(LED_YELLOW, HIGH);      // Liga o LED verde
        Serial.println("LED_YELLOW ligado!");
        Serial.println("umidade é moderada!");
        Serial.println("===================");// Imprime "LED verde ligado!"
    }
    else if((Temperature >= -10.1 && Temperature <= 50  ) && (Humidity >= 50.1 && Humidity <= 70))
    {
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_RED, LOW);     
        digitalWrite(LED_YELLOW, LOW);      
        Serial.println("LED_GREEN ligado!");
        Serial.println("umidade é moderada!");
        Serial.println("===================");// Imprime "LED verde ligado!"
    }
    else 
    {
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);     
        digitalWrite(LED_YELLOW, LOW);      
        Serial.println("Todos Desligados!");
        Serial.println("Temperatura irreais de se plantar!");
        Serial.println("===================");// Imprime "LED azul desligado!"
    }

    status = ThingSpeak.getLastReadStatus();  // Lê o status da última leitura no ThingSpeak

    if(status == 200)
    {
        Serial.println("Dados recebidos com sucesso");    // Imprime "Dados recebidos com sucesso"
    }
    else
    {
        Serial.println("Erro de envio " + String(status));// Imprime o erro de envio
    }

    Serial.println("---");

    delay(5000);  // Aguarda por 1 segundo antes de enviar os dados novamente
}
