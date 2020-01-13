void Rewind() {
  int BlocksPassed = 0;
  int TempBlock = 0;
  
  if (digitalRead(SPEED) == HIGH) //fast rewind
  {
    motorstatus = 4;
    Update_Motor_Status();
    CurrentTime = millis();
    do
    {
      //how many blocks have passed under read head
      BlocksPassed = (millis() - CurrentTime) / 220;

      TempBlock = CurrentBlock - BlocksPassed;

      if (CurrentTrack == 1)
      {
        if (TempBlock < 128)
          break;
      }
      else
      {
        if (TempBlock < 0)
          break;
      }
    } while (digitalRead(SPEED) == HIGH);

    //how many blocks have passed under read head
    BlocksPassed = (millis() - CurrentTime) / 220;

    CurrentBlock = CurrentBlock - BlocksPassed;
    
    if (CurrentTrack == 1)
    {
      if (CurrentBlock < 128)
      {
        CurrentBlock = 128;
        MOTORMOTION = false;
        motorstatus = 0;
        Update_Motor_Status();
        digitalWrite2f(MSENSEpin, LOW);
        __asm__("nop\n\t""nop\n\t");
      }
    }
    else
    {
      if (CurrentBlock < 0)
      {
        CurrentBlock = 0;
        MOTORMOTION = false;
        motorstatus = 0;
        Update_Motor_Status();
        digitalWrite2f(MSENSEpin, LOW);
        __asm__("nop\n\t""nop\n\t");
      }
    }
  }
  else
  {
    motorstatus = 2;
    Update_Motor_Status();
    CurrentTime = millis();
    do
    {
      //how many blocks have passed under read head
      BlocksPassed = (millis() - CurrentTime) / 300;

      TempBlock = CurrentBlock - BlocksPassed;

      if (CurrentTrack == 1)
      {
        if (TempBlock < 128)
          break;
      }
      else
      {
        if (TempBlock < 0)
          break;
      }
    } while (digitalRead2f(REVERSEpin) == LOW);
    //how many blocks have passed under read head
    BlocksPassed = (millis() - CurrentTime) / 300;

    CurrentBlock = CurrentBlock - BlocksPassed;
    
    if (CurrentTrack == 1)
    {
      if (CurrentBlock < 128)
      {
        CurrentBlock = 128;
        MOTORMOTION = false;
        motorstatus = 0;
        Update_Motor_Status();
        digitalWrite2f(MSENSEpin, LOW);
        __asm__("nop\n\t""nop\n\t");
      }
    }
    else
    {
      if (CurrentBlock < 0)
      {
        CurrentBlock = 0;
        MOTORMOTION = false;
        motorstatus = 0;
        Update_Motor_Status();
        digitalWrite2f(MSENSEpin, LOW);
        __asm__("nop\n\t""nop\n\t");
      }
    }
  }
}
