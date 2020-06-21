void ProcessButtons(){                                                // LCD and Button Routines
bool EQUAL = false;
    if (MOTORMOTION == false)         // Make sure we are not doing button and LED stuff while Command is waiting
    {
        //cycle files up
        reading = digitalRead2f(bUPpin);
        if (reading != lastbUPstate)
        {
          lastDebounceTimeUP = millis();
        }
        if ((millis() - lastDebounceTimeUP) > debounceDelay)
        {
          if (reading != buttonstateUP)
          {
            buttonstateUP = reading;
            if (buttonstateUP == LOW)
            {
              if (currentfile == numberoffiles){
                  currentfile = 1;
                  ddpfilename = GetFileName(filesindex[currentfile]);
              }
              else {
                  currentfile++;
                  ddpfilename = GetFileName(filesindex[currentfile]);
              }
              refreshscreen = 1;
            }
          }
        }
        lastbUPstate = reading;

        //cycle files down
        reading = digitalRead2f(bDOWNpin);
        if (reading != lastbDOWNstate)
        {
          lastDebounceTimeDOWN = millis();
        }
        if ((millis() - lastDebounceTimeDOWN) > debounceDelay)
        {
          if (reading != buttonstateDOWN)
          {
            buttonstateDOWN = reading;
            if (buttonstateDOWN == LOW)
            {
              if (currentfile == 1){
                  currentfile = numberoffiles;
                  ddpfilename = GetFileName(filesindex[currentfile]);
              }
              else {
                  currentfile--;
                  ddpfilename = GetFileName(filesindex[currentfile]);
              }
              refreshscreen = 1;
            }
          }
        }
        lastbDOWNstate = reading;

        //mount/unmount
        reading = digitalRead2f(bMOUNTpin);
        if (reading != lastbMOUNTstate)
        {
          lastDebounceTimeMOUNT = millis();
        }
        if ((millis() - lastDebounceTimeMOUNT) > debounceDelay)
        {
          if (reading != buttonstateMOUNT)
          {
            buttonstateMOUNT = reading;
            if (buttonstateMOUNT == LOW)
            {
              if (ddpregisterIt == 0)
                  {
                    ddpregisterIt = 1;
                    ddpfileIndex = filesindex[currentfile];
                    ddpfileNumber = currentfile;
                    ddpfilename = GetFileName(filesindex[currentfile]);
                    refreshscreen = 1;
                    if (CurrentTrack == 1)
                      CurrentBlock = 128;
                    else
                      CurrentBlock = 0;
                    LoadBlock(CurrentBlock, ddpfileIndex);
                    Serial.print("Mounted ");
                    Serial.println(ddpfilename);
                    digitalWrite2f(TAPEINpin, LOW);
                    __asm__("nop\n\t""nop\n\t");
                  }
              else
                  {
                    Serial.println("UnMounted");
                    ddpregisterIt = 0;
                    ddpfileIndex = 0;
                    ddpfileNumber = 0;
                    refreshscreen = 1;
                    digitalWrite2f(TAPEINpin, HIGH);
                    __asm__("nop\n\t""nop\n\t");
                    detachInterrupt(digitalPinToInterrupt(2));
                  }
            }
          }
        }
        lastbMOUNTstate = reading;

        //mode toggle
        reading = digitalRead2f(bMODEpin);
        if (reading != lastbMODEstate)
        {
          lastDebounceTimeMODE = millis();
        }
        if ((millis() - lastDebounceTimeMODE) > debounceDelay)
        {
          if (reading != buttonstateMODE)
          {
            buttonstateMODE = reading;
            if (buttonstateMODE == LOW)
            {
              TAPEGWMODE = !TAPEGWMODE;
              if (TAPEGWMODE)
                tapemode = "RD";
              else
                tapemode = "CD";
              refreshscreen = 1;
            }
          }
        }
        lastbMODEstate = reading;
        
        if (refreshscreen == 1)
        {
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(2);
          display.setCursor(0,0);
          display.print(tapemode);
          display.setCursor(42,0);
          display.print(motor_status[motorstatus]);
          display.setTextSize(1);
          display.setCursor(0,32);
          if (currentfile == ddpfileNumber)
            display.setTextColor(BLACK,WHITE);
          else
            display.setTextColor(WHITE);
          display.println(ddpfilename);
          display.display();
          refreshscreen = 0;
        }
    }
}

void Update_Motor_Status() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print(tapemode);
  display.setCursor(42,0);
  display.print(motor_status[motorstatus]);
  display.setTextSize(1);
  display.setCursor(0,32);
  if (currentfile == ddpfileNumber)
    display.setTextColor(BLACK,WHITE);
  else
    display.setTextColor(WHITE);
  display.println(ddpfilename);
  display.display();
}

void Update_Show_Track() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print(tapemode);
  display.setCursor(42,0);
  display.print(motor_status[motorstatus]);
  display.print(CurrentBlock);
  display.display();
}
