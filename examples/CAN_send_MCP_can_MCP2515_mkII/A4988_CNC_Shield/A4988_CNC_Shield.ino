// Run a A4998 Stepstick from an Arduino UNO.
// Paul Hurley Aug 2015
int x; 
#define BAUD (115200)
#define EnablePIN 8
#define DirPIN 6 // 2// 3 //4
#define StepPIN 3 // 5 //6 //7
uint8_t DIR = LOW;


void setup() 
{
  SerialUSB.begin(BAUD);
  pinMode(EnablePIN,OUTPUT); // Enable
  pinMode(StepPIN,OUTPUT); // Step
  pinMode(DirPIN,OUTPUT); // Dir
  digitalWrite(EnablePIN,LOW); // Set Enable low
}

void loop() 
{
    digitalWrite(EnablePIN,LOW); // Set Enable low
  DIR = !DIR;
  digitalWrite(DirPIN,DIR); // Set Dir high
  SerialUSB.println("Loop 200 *16 steps (1 rev)");
  for(x = 0; x < 200 * 16; x++) // Loop 200 times
  {
    digitalWrite(StepPIN,HIGH); // Output high
    delay(1); // Wait
    digitalWrite(StepPIN,LOW); // Output low
    //delay(1); // Wait
  }
  SerialUSB.println("Pause");
  delay(1000); // pause one second
}
