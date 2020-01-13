void Stop()
{
  pinMode(TXpin, INPUT_PULLUP);
  digitalWrite2f(MSENSEpin, LOW);
  //detachInterrupt(digitalPinToInterrupt(2));
  detachInterrupt(digitalPinToInterrupt(3));
  STOPPED = true;
  UPDATESTATUS = true;
  MOTORMOTION = false;
  FORWARDMOTION = false;
  REVERSEMOTION = false;
  motorstatus = 0;
}
