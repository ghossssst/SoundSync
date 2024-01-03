int rightsensorpin = A0;   
int leftsensorpin = A1;
int microphonesensitivity = 20; 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);

  if (analogRead(rightsensorpin)>microphonesensitivity)
  {
    digitalWrite(LED_BUILTIN, LOW); 
    Serial.println(analogRead(rightsensorpin));
    digitalWrite(19, HIGH);
    delay(1000);  
  }

  if (analogRead(leftsensorpin)>microphonesensitivity)
  {
    digitalWrite(LED_BUILTIN, LOW); 
    Serial.println(analogRead(leftsensorpin));
    digitalWrite(18, HIGH);
    delay(1000);  
  }

  digitalWrite(19, LOW); 
  digitalWrite(18, LOW);
}
