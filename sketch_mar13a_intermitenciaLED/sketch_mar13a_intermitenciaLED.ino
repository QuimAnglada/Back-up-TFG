//Intermitència del LED

void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT); // el primer argument s'ha declarat com a sortida.

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(3, HIGH); // encedre un pin digital, el pin és el primer argument, encendre és el segon (HIGH).
  delay(1000); // Perquè hi ha intermitència, l'argument és el temps, expressat en ms, que la llum està apagada.  
  digitalWrite(3, LOW); // apagar un pin digital (LOW).
  delay(1000); 
  // és IR :(
}
