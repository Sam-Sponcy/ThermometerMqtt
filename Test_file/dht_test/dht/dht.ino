#include <DHT.h>

#define DHTPIN 2     // Broche à laquelle est connecté le capteur
#define DHTTYPE DHT11   // Type de capteur (DHT11, DHT22, AM2302, etc.)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  // Initialisation de la communication série
  dht.begin();         // Initialisation du capteur
}

void loop() {
  // Lecture de la température et de l'humidité
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Vérification si la lecture a réussi
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erreur lors de la lecture du capteur !");
    return;
  }

  // Affichage des valeurs lues
  Serial.print("Température: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidité: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000);  // Attente de 2 secondes avant la prochaine lecture
}
