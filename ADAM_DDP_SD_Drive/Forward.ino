void Forward() {
  int BlocksPassed = 0;
  int TempBlock = 0;

    pinMode(TXpin, OUTPUT);
    
     //send block
    SendHeaderASM();

    delay(100);
    SendDataASM();

    delay(2);
    SendCRCASM();

    //increment block
    CurrentBlock++;

    return;
}

void FastForward() {
  int BlocksPassed = 0;
  int TempBlock = 0;

    CurrentTime = millis();
    do
    {
      //how many blocks have passed under read head
      BlocksPassed = (millis() - CurrentTime) / 220;

      TempBlock = CurrentBlock + BlocksPassed;

      if (CurrentTrack == 1)
      {
        if (TempBlock > 255)
          break;
      }
      else
      {
        if (TempBlock > 127)
          break;
      }
    } while (digitalRead2f(FORWARDpin) == LOW);

    //how many blocks have passed under read head
    BlocksPassed = (millis() - CurrentTime) / 220;

    CurrentBlock = CurrentBlock + BlocksPassed;

    if (CurrentTrack == 1)
    {
      if (CurrentBlock > 255)
      {
        CurrentBlock = 255;
        MOTORMOTION = false;
        motorstatus = 0;
        Update_Motor_Status();
        digitalWrite2f(MSENSEpin, LOW);
      }
    }
    else
    {
      if (CurrentBlock > 127)
      {
        CurrentBlock = 127;
        MOTORMOTION = false;
        motorstatus = 0;
        Update_Motor_Status();
        digitalWrite2f(MSENSEpin, LOW);
      }
    }
    LoadBlock(CurrentBlock,ddpfileIndex);  
}
