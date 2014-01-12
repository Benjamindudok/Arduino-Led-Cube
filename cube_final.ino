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

void setup() {                
  // Openzetten van alle digitale porten om de LED cube aan te sluiten
  pinMode(13, OUTPUT);     
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  
  // Open zetten van de poorten om de shift register te kunnen gebruiken
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);  
  pinMode(latch, OUTPUT); 
  
  // Open zetten van de poorten voor de MSEQ7 sensor aan te sluiten en het analoge signaal op te vangen
  Serial.begin(9600);
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);

// beginwaarde aangeven voor de reset en strobe poort voor de MSEQ7
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  
}

// Onderstaande code is om de 595 shift register te initialiseren en ervoor te zorgen dat hij automatisch LEDS kan updaten.

/*
 * updateLEDs() - updates de status van ledState() op de 595 shift register
 */
void updateLEDs(int value){
  digitalWrite(latch, LOW);     //Zet de chips 'latch' op LOW
  shiftOut(data, clock, MSBFIRST, value); //zend 8-bits informatie naar de chip (het 'shiften')
  digitalWrite(latch, HIGH);   //Zet de chips 'latch' op HIGH waardoor de bit-waarden worden laten zien
}

/*
 * updateLEDsLong() - Zend de LED status in ledStates naar de 595, maar op deze manier wordt het shiften van de states 
 * softwarematig gedaan in plaats van in de chip zelf. (wordt op het moment niet gebruikt)
 */ 
void updateLEDsLong(int value){
  digitalWrite(latch, LOW);    //Zet de chips 'latch' op LOW
  for(int i = 0; i < 8; i++){  //Wisselt tussen de 8 bits
  int bit = value & B10000000; // Gebruikt een 'bitmask' om het achtse bit te selecteren in het nummer
  value = value << 1;          //en daarna wordt er 1 bij het nummer opgeteld zodat de volgende keer bit 7 aan de beurt is.
  if(bit == 128){digitalWrite(data, HIGH);} //Als bit 8 is geselecteerd gaat de data pin op HIGH
  else{digitalWrite(data, LOW);}            //als hij niet is geselecteerd gaat hij op LOW
  digitalWrite(clock, HIGH);                //De volgende 3 regels zorgen ervoor dat de tijdsinterval 'clock' gaat werken.
  delay(1);
  digitalWrite(clock, LOW);
  }
  digitalWrite(latch, HIGH);  //Zet de chips 'latch' op HIGH waardoor de bit-waarden worden laten zien
}


//Onderstaande wordt gebruikt in de 'Bitwise' berekening om de LEDS te laten oplichten
// voor meer informatie http://en.wikipedia.org/wiki/Bitwise_operation
int bits[] = {B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000};
int masks[] = {B11111110, B11111101, B11111011, B11110111, B11101111, B11011111, B10111111, B01111111};
/*
 * changeLED(int led, int state) - deze functie verandert een individuele LED
 * LEDs zijn 0 tot 7 en de status is 0 - OFF of 1 - ON
 */
 void changeLED(int led, int state){
   ledState = ledState & masks[led];  //leegt de ledState
   if(state == ON){ledState = ledState | bits[led];} //Als de bit aan staat, wordt hij toegevoegd aan de ledState
   updateLEDs(ledState);              //zend de nieuwe ledState naar het register
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
      Serial.print(spectrumValue[i]); //print de waarde in de serial monitor
      digitalWrite(13, HIGH);   // LED rij aan
      digitalWrite(12, LOW);   // LED rij uit
      digitalWrite(11, LOW);   // LED rij uit
      digitalWrite(10, LOW);   // LED rij uit
      digitalWrite(9, LOW);   // LED rij uit
  
      digitalWrite(8, LOW);   // LED rij uit
      digitalWrite(7, LOW);   // LED rij uit
      digitalWrite(6, LOW);   // LED rij uit
      digitalWrite(3, LOW);   // LED rij uit
      digitalWrite(2, LOW);   // LED rij uit
  
      // stuk code voor de shift register, deze shift register regelt de stroomtoevoer naar de verdiepingen (werkt niet door soldeerfout)
      for(int f = 0; f < 8; f++){ 
        changeLED(0,ON); // 'begane grond' toevoer
        changeLED(1,ON); // eerste verdieping toevoer
        changeLED(2,ON); // tweede verdieping toevoer
        changeLED(3,ON); // derde verdieping toevoer
        changeLED(4,ON); // vierde verdieping toevoer
      }
    }
    else if (spectrumValue[i] < 100  ) // Als de waarde kleiner is dan 100 dan gaan er 2 rijen met LEDs aan.
    {
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   // LED rij aan
      digitalWrite(12, HIGH);   // LED rij aan
      digitalWrite(11, LOW);   // LED rij uit
      digitalWrite(10, LOW);   // LED rij uit
      digitalWrite(9, LOW);   // LED rij uit
  
      digitalWrite(8, LOW);   // LED rij uit
      digitalWrite(7, LOW);   // LED rij uit
      digitalWrite(6, LOW);   // LED rij uit
      digitalWrite(3, LOW);   // LED rij uit
      digitalWrite(2, LOW);   // LED rij uit
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 120 ) // Als de waarde kleiner is dan 120 dan gaan er 3 rijen met LEDs aan
    {
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, LOW);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   // LED rij uit
      digitalWrite(7, LOW);   // LED rij uit
      digitalWrite(6, LOW);   // LED rij uit
      digitalWrite(3, LOW);   // LED rij uit
      digitalWrite(2, LOW);   // LED rij uit
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 140 ) // Als de waarde kleiner is dan 140 dan gaan er 4 rijen met LEDs aan
    {
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   // LED rij uit
      digitalWrite(7, LOW);   // LED rij uit
      digitalWrite(6, LOW);   // LED rij uit
      digitalWrite(3, LOW);   // LED rij uit
      digitalWrite(2, LOW);   // LED rij uit
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 160 ) // Als de waarde kleiner is dan 160 dan gaan alle 5 de rijen met LEDs aan
    {
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, LOW);   // LED rij uit
      digitalWrite(7, LOW);   // LED rij uit
      digitalWrite(6, LOW);   // LED rij uit
      digitalWrite(3, LOW);   // LED rij uit
      digitalWrite(2, LOW);   // LED rij uit
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else // bij alle anders waardes gebeurt er niets. De sensor waarde komt namelijk nooit hoger dan 160 dus zolang er iets te horen is, gaan er altijd LEDs aan.
    {
      Serial.print(spectrumValue[i]);
    }
    
    digitalWrite(strobePin, HIGH);
  }
  Serial.println(); // nadat alle if's zijn doorgelopen print hij de waardes rechtstreeks naar de serial monitor.
  
  // hierna worden alle LEDs gereset door alles uit te gooien.
  // BELANGRIJK: MORGEN OCHTEND PROBEREN ONDERSTAANDE CODE UIT TE ZETTEN OM TE PROBEREN HET FLIKKEREN WEG TE HALEN
      digitalWrite(13, LOW);   // LED rij uit // LAATSTE UPDATE STOND OP HIGH
      digitalWrite(12, LOW);   // LED rij uit
      digitalWrite(11, LOW);   // LED rij uit
      digitalWrite(10, LOW);   // LED rij uit
      digitalWrite(9, LOW);   // LED rij uit
  
      digitalWrite(8, LOW);   // LED rij uit
      digitalWrite(7, LOW);   // LED rij uit
      digitalWrite(6, LOW);   // LED rij uit
      digitalWrite(3, LOW);   // LED rij uit
      digitalWrite(2, LOW);   // LED rij uit
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
  
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
      Serial.print("    ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, HIGH);   
      digitalWrite(7, HIGH);   
      digitalWrite(6, HIGH);   
      digitalWrite(3, HIGH);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 100  )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, HIGH);   
      digitalWrite(7, HIGH);   
      digitalWrite(6, HIGH);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 120 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);  
      digitalWrite(12, HIGH);  
      digitalWrite(11, HIGH);  
      digitalWrite(10, HIGH);  
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, HIGH);   
      digitalWrite(7, HIGH);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 140 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, HIGH);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 160 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else
    {
      Serial.print("  ");
      Serial.print(spectrumValue[i]);
      
    }
    
    digitalWrite(strobePin, HIGH);
  }
  Serial.println();digitalWrite(13, HIGH);   
      digitalWrite(12, LOW);   
      digitalWrite(11, LOW);   
      digitalWrite(10, LOW);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
  
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
      Serial.print("    ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, LOW);   
      digitalWrite(10, LOW);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, HIGH);   
      digitalWrite(3, HIGH);   
      digitalWrite(2, HIGH);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 100  )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, LOW);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, HIGH);   
      digitalWrite(2, HIGH);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 120 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, HIGH);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 140 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, HIGH);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 160 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else
    {
      Serial.print("  ");
      Serial.print(spectrumValue[i]);
      
    }
    
    digitalWrite(strobePin, HIGH);
  }
  Serial.println();digitalWrite(13, HIGH);   
      digitalWrite(12, LOW);   
      digitalWrite(11, LOW);   
      digitalWrite(10, LOW);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
  
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
      Serial.print("    ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, LOW);   
      digitalWrite(11, LOW);   
      digitalWrite(10, LOW);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 100  )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, HIGH);   
      digitalWrite(7, HIGH);   
      digitalWrite(6, HIGH);   
      digitalWrite(3, HIGH);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 120 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, LOW);   
      digitalWrite(12, LOW);   
      digitalWrite(11, LOW);   
      digitalWrite(10, LOW);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 140 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, HIGH);   
      digitalWrite(6, HIGH);   
      digitalWrite(3, HIGH);   
      digitalWrite(2, HIGH);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else if (spectrumValue[i] < 160 )
    {
      Serial.print("   ");
      Serial.print(spectrumValue[i]);
      digitalWrite(13, HIGH);   
      digitalWrite(12, HIGH);   
      digitalWrite(11, HIGH);   
      digitalWrite(10, HIGH);   
      digitalWrite(9, HIGH);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
    }
    else
    {
      Serial.print("  ");
      Serial.print(spectrumValue[i]);
      
    }
    
    digitalWrite(strobePin, HIGH);
  }
  Serial.println();digitalWrite(13, HIGH);   
      digitalWrite(12, LOW);   
      digitalWrite(11, LOW);   
      digitalWrite(10, LOW);   
      digitalWrite(9, LOW);   
  
      digitalWrite(8, LOW);   
      digitalWrite(7, LOW);   
      digitalWrite(6, LOW);   
      digitalWrite(3, LOW);   
      digitalWrite(2, LOW);   
  
      for(int f = 0; f < 8; f++){
        changeLED(0,ON);
        changeLED(1,ON);
        changeLED(2,ON);
        changeLED(3,ON);
        changeLED(4,ON);
      }
  
}





