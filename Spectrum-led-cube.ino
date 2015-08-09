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

int pinOuts[10] = {13, 12, 11, 10, 9, 8, 7, 6, 3, 2};

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
  
  animatieA();  
}
 
/* Hier beginnen de animaties van de kubus */

// AnimatieA laat een standaard visualizer zien die van links naar rechts beweegt op de maat van de muziek.

void animatieA() {
  //stukje code dat de MSEQ7 nodig heeft om de variabel 'spectrumValue' te vullen
  digitalWrite(resetPin, HIGH); // resetpin naar HIGH om een nieuwe variabel te kunnen bereken 
  digitalWrite(resetPin, LOW); // en de resetpin weer uit om de variabel te vullen

  for (int i = 0; i < 7; i++) // maakt 7 kanalen aan voor in de variabel
  {
    digitalWrite(strobePin, LOW); //stoppen met wisselen van kanalen
    delayMicroseconds(30); // geeft de output de tijd om te stoppen met schommelen
    spectrumValue[i] = analogRead(analogPin); // slaat alle informatie die is gelezen uit de sensor op in de verschillende kanalen
    
    if (spectrumValue[i] < 80) // Als de waarde kleiner is dan 80, dan gaan de onderstaande rij codes aan die de LEDS aansturen
    {
      bool value[] = {HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 100  ) // Als de waarde kleiner is dan 100 dan gaan er 2 rijen met LEDs aan.
    {
      bool value[] = {HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 120 ) // Als de waarde kleiner is dan 120 dan gaan er 3 rijen met LEDs aan
    {
     bool value[] = {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 140 ) // Als de waarde kleiner is dan 140 dan gaan er 4 rijen met LEDs aan
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 160 ) // Als de waarde kleiner is dan 160 dan gaan alle 5 de rijen met LEDs aan
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    
    digitalWrite(strobePin, HIGH);
  }
  
  bool value[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
  setLEDRows(value);
}

// Onderstaande functie is een tweede animatie programma waarin dezelfde animatie wordt weergegeven als 
// in A maar dan in een andere richting. Onderstaande code is niet gecomment omdat het hetzelfde werkt als animatieA.

void animatieB() {
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); 
    spectrumValue[i] = analogRead(analogPin);

    if (spectrumValue[i] < 80)
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW};
      setLEDRows(value); 

    }
    else if (spectrumValue[i] < 100  )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW};
      setLEDRows(value); 
    }
    else if (spectrumValue[i] < 120 )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 140 )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW};
      setLEDRows(value); 
    }
    else if (spectrumValue[i] < 160 )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);  
    }
    
    digitalWrite(strobePin, HIGH);
  }
  
  bool value[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
  setLEDRows(value);  
}

// Onderstaande functie is animatie programma C. Wederom niet gecomment omdat het hetzelfde werkt als A. Het patroon is ditmaal wel heel anders.

void animatieC() {
    digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30);
    spectrumValue[i] = analogRead(analogPin);

    if (spectrumValue[i] < 80)
    {
      bool value[] = {HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 100  )
    {
      bool value[] = {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH};
      setLEDRows(value); 
    }
    else if (spectrumValue[i] < 120 )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 140 )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 160 )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);    
    }
    else
    {
      Serial.print("  ");
      Serial.print(spectrumValue[i]);
      
    }
    
    digitalWrite(strobePin, HIGH);
  }
  
  bool value[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
  setLEDRows(value);  
  
  
}

// Onderstaande functie is animatie programma D. Wederom niet gecomment omdat het hetzelfde werkt als A. Het patroon is dit keer wel anders.

void animatieD() {
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); 
    spectrumValue[i] = analogRead(analogPin);

    if (spectrumValue[i] < 80)
    {
      bool value[] = {HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 100  )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW};
      setLEDRows(value); 
    }
    else if (spectrumValue[i] < 120 )
    {
      bool value[] = {LOW, LOW, LOW, LOW, HIGH, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 140 )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, HIGH, HIGH};
      setLEDRows(value);
    }
    else if (spectrumValue[i] < 160 )
    {
      bool value[] = {HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW};
      setLEDRows(value);
    }
    
    digitalWrite(strobePin, HIGH);
  }
  
  bool value[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
  setLEDRows(value); 
}

void setLEDRows(bool value[]) {
  for (int i = 2; i < 10; i++) { 
    digitalWrite(pinOuts[i], value[i]);
  } 
}



