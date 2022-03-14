//Activació dels relés

void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT); // el primer argument és pin de sortida, en aquest cas el relé 1 -> 32, relé 2 -> 33.

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(3, HIGH); // encedre un pin digital, el pin és el primer argument, encendre és el segon (HIGH).
  delay(1000); // Perquè hi ha intermitència, l'argument és el temps, expressat en ms, en que el relé i la seva llum està apagades.  
  digitalWrite(3, LOW); // apagar un pin digital (LOW).
  delay(1000); 
  // El botó inferior atura la seqüència, el botó superior activa el bluetooth ?
}
