 //LORA MASSAGING SYSTEM BY RANPEWD & ISSOUPEWD

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "keyboard.h"
#include <LoRa.h>
#include <SPI.h>
#include <vector>

#define SCREEN_WIDTH 128  // Largeur de l'écran
#define SCREEN_HEIGHT 64  // Hauteur de l'écran

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Définir une résolution plus petite pour l'écran OLED 0.96"
#define OLED_WIDTH 128  // Largeur de l'écran OLED 0.96"
#define OLED_HEIGHT 64  // Hauteur de l'écran OLED 0.96"

const int csPin = 17;
const int resetPin = 22;
const int irqPin = 5;
int SyncWord = 0x22;

bool KeyShift = false;

// Declare a global vector to store received messages
std::vector<String> receivedMessages;

String myMessage = "";
char input;

int screen = 0;
int lastscreen = -1;
int pew = 0;

const int buzzerPin = 13; // Définir le numéro de broche pour le buzzer

void setup() {
  Wire.setSDA(20);
  Wire.setSCL(21);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("LORA Messenger");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(r0, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);

  pinMode(keybC[0], INPUT_PULLUP);
  pinMode(keybC[1], INPUT_PULLUP);
  pinMode(keybC[2], INPUT_PULLUP);
  pinMode(keybC[3], INPUT_PULLUP);
  pinMode(keybC[4], INPUT_PULLUP);
  pinMode(keybC[5], INPUT_PULLUP);
  pinMode(keybC[6], INPUT_PULLUP);
  pinMode(keybC[7], INPUT_PULLUP);
  pinMode(keybC[8], INPUT_PULLUP);
  pinMode(keybC[9], INPUT_PULLUP);



  // Initialisation du module LoRa en tant qu'émetteur
  LoRa.setPins(csPin, resetPin, irqPin);
  LoRa.begin(433E6);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  LoRa.setCodingRate4(8);
  LoRa.setSyncWord(SyncWord);

  // Initialisation de l'écran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Échec de l'initialisation de l'écran SSD1306"));
    for (;;)
      ;
  }

  printBoot();
  delay(3000);
  printHome();
  muncont();
}

void loop() {
  input = readKeyboard(KeyShift);

  if (input != '\0') {
    Serial.println("start");
    if (input == '\a') {
      KeyShift = !KeyShift;
      ran++;
      if (ran > 3) {
        ran = 1;
      }
    } else if (input == '\b') {
      if (myMessage.length() > 0) {
        myMessage.remove(myMessage.length() - 1);
      }
    } else if (input == '\n') {
      // Envoi du message
      Serial.println("envoi de message");
      if (myMessage.length() > 0) {
        LoRa.beginPacket();
        Serial.println("in lora");
        LoRa.print(myMessage);
        Serial.println("msg lora");
        LoRa.endPacket();
        Serial.println("out lora");
        myMessage = "";
        prints();
      }
    } else {
      myMessage = myMessage + input;
    }
  

    if (screen == 0) {
      printHome();
      muncont();
    } else if (screen == 1) {
      prints();
    } else if (screen == 2) {
      printLecture();
    }
  }

  if (screen == 1 || screen == 2) {
    if (input == '\t') {
      screen = 0;
    }
  }
   if (LoRa.parsePacket()) {
    // String received = ""; // Remove this line
    while (LoRa.available()) {
      receivedMessages.push_back(LoRa.readString()); // Update the global vector
    }
    Serial.print("Received packet: ");
    Serial.println(receivedMessages.back()); // Print the last received message
    
    // Activer le buzzer lorsque vous recevez un message
    playMelody(); // Jouer la mélodie
  }   
}





void printBoot() {
  display.clearDisplay();               // Clear the display
  display.setTextColor(SSD1306_WHITE);  // Set text color to white
                                        // Display a loading animation
  int x = 0;
  int y = 20;
  int spacing = 25;  // Spacing between each dot
  for (int i = 0; i < 9; i++) {
    display.clearDisplay();  // Clear the display
    display.setCursor(x, y);
    display.setTextSize(2);
    display.print("Loading");
    display.setCursor(x + 40, y);
    display.setTextSize(2);
    display.print("...");
    display.display();
    x += 4;             // Move the animation slightly to the right
    display.display();  // Update the display
    delay(100);         // Adjust the delay to control the speed of animation
  }




  // Position et taille du texte ajustées pour un écran OLED de 0,96 pouce
  display.clearDisplay();               // Efface l'écran
  display.setTextColor(SSD1306_WHITE);  // Couleur du texte en blanc
  display.setTextSize(1);               // Taille de police
  // Affichage du texte "Welcome to"
  display.setCursor(0, 0);  // Position du premier texte
  display.println("Welcome to");
  // Affichage du texte "LoRaMessenger"
  display.setCursor(10, 25);  // Position du deuxième texte
  display.println("   LoRaMessenger");
  // Affichage du texte "by Rania & Kahina"
  display.setTextSize(1);    // Réduction de la taille de police pour le troisième texte
  display.setCursor(0, 55);  // Position du troisième texte
  display.println("    by pewdsNETWORK  ");
  display.display();  // Affiche le contenu de l'écran
}



void printHome() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);  // Taille de police par défaut
  display.print("LoRa Messenger      ");
  display.print(ran);
  display.display();
}




void prints() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setTextSize(0.5);  // Taille de police par défaut
  display.print("ECRIRE :            ");
  display.print(ran);
  display.setTextColor(SSD1306_WHITE);
  display.println(myMessage);
  display.display();
}

void printLecture() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("LECTURE          ");
  display.print(ran);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.setTextSize(1);

  
  // Iterate through the vector and display each received message
 if (receivedMessages.size() < 4) {
      for (size_t i = 0; i < receivedMessages.size(); i++) {
    display.println(receivedMessages[i]);
  }
  }

 else if (receivedMessages.size() >= 4) {
   display.println(receivedMessages.back());
    receivedMessages.clear();
  }

  display.display();
}

void muncont() {
  // Incrémente pew lorsque '\f' est détecté
  if (input == '\f') {
    pew++;
  }

  // Remise à zéro de pew si elle atteint 2
  if (pew == 2) {
    pew = 0;
  }

  // Si pew est égal à 0, affiche le menu pour écrire un message
  if (pew == 0) {
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    display.setCursor(10, 20);
    display.print(" >Ecrire un message");
    display.setCursor(10, 40);
    display.println("  Lire les messages");

    // Si '\v' est détecté, passe à l'écran pour écrire un message
    if (input == '\v') {
      screen = 1;
      myMessage = "";
    }
  }

  // Si pew est égal à 1, affiche le menu pour lire les messages
  if (pew == 1) {
    display.setCursor(10, 20);
    display.println("  Ecrire un message");
    display.setCursor(10, 40);
    display.print(" >Lire les messages");

    // Si '\v' est détecté, passe à l'écran pour lire les messages
    if (input == '\v') {
      screen = 2;
      myMessage = "";
    }
  }
  display.display();
}

// Fonction pour activer le buzzer pendant une certaine durée
void buzz(int buzzerPin, int frequency, int duration) {
  tone(buzzerPin, frequency); // Générer un signal PWM à une fréquence spécifiée
  delay(duration);            // Maintenir le buzzer actif pendant la durée spécifiée
  noTone(buzzerPin);          // Arrêter le buzzer après la durée spécifiée
}

// Fonction pour jouer une mélodie simple
void playMelody() {
  buzz(buzzerPin, 1000, 200); // Première note
  delay(50);                  // Pause entre les notes
  buzz(buzzerPin, 1500, 200); // Deuxième note
  delay(50);                  // Pause entre les notes
  buzz(buzzerPin, 2000, 200); // Troisième note
}
