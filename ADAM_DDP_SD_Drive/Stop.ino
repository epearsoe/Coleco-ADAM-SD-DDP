void Stop()
{
  detachInterrupt(digitalPinToInterrupt(3));
  STOPPED = true;
  READSTOP = true;
  UPDATESTATUS = true;
  MOTORMOTION = false;
  FORWARDMOTION = false;
  REVERSEMOTION = false;
  motorstatus = 0;
  digitalWrite(TXpin, LOW);
  PORTD = PORTD & 0b01111111;                 // Set MSENSE PD7 = LOW
}
