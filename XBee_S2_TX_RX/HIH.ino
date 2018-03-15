//HIH
const int outPin = A7; // (7) pin for the analogue input

void HIH(int *value)
{
  *value = analogRead(outPin); // read the voltage on the out pin
}
