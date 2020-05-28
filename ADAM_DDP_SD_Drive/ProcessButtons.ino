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
              if (DEBUG)
              {
                MONITORON = !MONITORON;
              }
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
        
        if (refreshscreen == 1)
        {
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(2);
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
  display.setCursor(42,0);
  display.print(motor_status[motorstatus]);
  display.print(CurrentBlock);
  display.display();
}

void Report_Status() {
  while (NOTDONE)
  {
    ShowStatus();
  }
}

void ShowStatus() {
  //reading = digitalRead2f(BRAKEpin);
  //if (reading == HIGH) {status[0] = "BRK";} else {status[0] = "OFF";}
  
  reading = digitalRead2f(STOPpin);
  if (reading == HIGH) {status[0] = "STOP";} else {status[1] = "OFF";}
  
  reading = digitalRead2f(REVERSEpin);
  if (reading == LOW) {status[1] = "REV";} else {status[2] = "OFF";}
  
  reading = digitalRead2f(FORWARDpin);
  if (reading == LOW) {status[2] = "FWD";} else {status[3] = "OFF";}
  
  reading = digitalRead2f(SPEEDpin);
  if (reading == HIGH) {status[3] = "FAST";} else {status[4] = "SLOW";}
  
  reading = digitalRead2f(MODEpin);
  if (reading == HIGH) {status[4] = "READ";} else {status[5] = "WRITE";}
  
  reading = digitalRead2f(TRACKpin);
  if (reading == HIGH) {status[5] = "A";} else {status[6] = "B";}

  reading = digitalRead2f(MSENSEpin);
  if (reading == HIGH) {status[6] = "MOVE";} else {status[7] = "|";}
  
  reading = digitalRead2f(TAPEINpin);
  if (reading == LOW) {status[7] = "TIN";} else {status[8] = "TOUT";}

  reading = digitalRead2f(TX_pin);
  if (reading == LOW) {status[8] = "TXLOW";} else {status[9] = "TXHIGH";}

  //reading = digitalRead2f(NOCONpin);
  //if (reading == LOW) {status[10] = "NCLOW";} else {status[10] = "NCHIGH";}

  reading = digitalRead(RXpin);
  if (reading == LOW) {status[9] = "RXLOW";} else {status[11] = "RXHIGH";}

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  for (int j=0; j<8; j++)
  {
    display.println(status[j]);
  }
  display.print(status[8]);
  display.print("    ");
  display.println(status[9]);
  display.display();
}
