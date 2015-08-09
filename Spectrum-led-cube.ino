/*
  Arduino LED Cube spectrum visualizer v1.0
  Met onderstaande code kan je een 5x5x5 LED cube aansluiten en een patroon laten afspelen dat reageert op de toonhoogtes 
  en sterktes die worden gemeten met een MSEQ7 Spectrum Analyzer. In deze code zit nog geen mogelijkheid om automatisch van
  patroon te wisselen, maar dit kan wel handmatig worden gedaan.
  
 */
  
// instellen van namen voor de porten die de 595 shift register
int data = A4; 
int clock = A5;
int latch = A3;

// variabelen om te gebruiken met de shift register. Dit is speciaal om enkele LEDS aan te kunnen sturen
int ledState = 0;
const int ON = HIGH;
const int OFF = LOW;

int analogPin = A0; // pin die wordt gebruikt om het analoge signaal van de MSEQ7 op te vangen in de arduino
int strobePin = 4; // Digitale pin die wordt gebruikt om van kanaal te wissel in de MSEQ7
int resetPin = 5; // Digitale pin om de MSEQ7 te resetten, anders blijven de waardes zich bij elkaar optellen.
int spectrumValue[7]; // Om de uiteindelijke waardes uit de MSEQ7 op te slaan in een variabel
int spectrumIncrements[5] = {80, 100, 120, 140, 160};

int pinOuts[10] = {13, 12, 11, 10, 9, 8, 7, 6, 3, 2};

bool pattern1[5][10] = {
  {HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW},
  {HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW},
  {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW},
  {HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW},
  {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW}
};

bool pattern2[5][10] = {
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW},
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW},
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW},
  {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW}
};

bool pattern3[5][10] = {
  {HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH},
  {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH},
  {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH},
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW},
  {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW}
};

bool pattern4[5][10] = {
  {HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW},
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},
  {LOW, LOW, LOW, LOW, HIGH, LOW, LOW, LOW, LOW, LOW},
  {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, HIGH, HIGH},
  {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW}
};

void setup() {                
  // Openzetten van alle digitale porten om de LED cube aan te sluiten  
  for (int i = 2; i < 10; i++) { 
        pinMode(i, OUTPUT);
  }
  
  // Open zetten van de poorten om de shift register te kunnen gebruiken
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);  
  pinMode(latch, OUTPUT); 
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  
  // Open zetten van de poorten voor de MSEQ7 sensor aan te sluiten en het analoge signaal op te vangen
  Serial.begin(9600);

// beginwaarde aangeven voor de reset en strobe poort voor de MSEQ7
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  
} 
 
 void loop() {
  // Hier kan je de verschillende animatiefuncties oproepen die je kan laten zien. Het werkt niet om een 
  // delay erna te zetten en dan een ander animatie programma op te roepen. Want in principe is een animatie 
  // programma wat hierboven staat 1 waarde die hij uitleest en dus laat hij ook maar 1x een blink zien waar die waarde bij hoort
  
  animate(pattern1);  
}
 
/* Hier beginnen de animaties van de kubus */

// AnimatieA laat een standaard visualizer zien die van links naar rechts beweegt op de maat van de muziek.

void animate(bool pattern[5][10]) {
  //stukje code dat de MSEQ7 nodig heeft om de variabel 'spectrumValue' te vullen
  digitalWrite(resetPin, HIGH); // resetpin naar HIGH om een nieuwe variabel te kunnen bereken 
  digitalWrite(resetPin, LOW); // en de resetpin weer uit om de variabel te vullen

  for (int i = 0; i < 7; i++) // maakt 7 kanalen aan voor in de variabel
  {
    digitalWrite(strobePin, LOW); //stoppen met wisselen van kanalen
    delayMicroseconds(30); // geeft de output de tijd om te stoppen met schommelen
    spectrumValue[i] = analogRead(analogPin); // slaat alle informatie die is gelezen uit de sensor op in de verschillende kanalen
    
    for (int j = 0; j < 5; j++) {
      if (spectrumValue[i] < spectrumIncrements[j]) // Als de waarde kleiner is dan 80, dan gaan de onderstaande rij codes aan die de LEDS aansturen
        setLEDRows(pattern[j]);
    }
    
    digitalWrite(strobePin, HIGH);
  }
  
  // reset outputs
  bool value[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
  setLEDRows(value);
}

void setLEDRows(bool value[]) {
  for (int i = 2; i < 10; i++) { 
    digitalWrite(pinOuts[i], value[i]);
  } 
}
