void SendHeaderASM()
{
  int bits = 7;

  //Serial.println(CurrentBlock);
  Update_Show_Track();
  for (int i=6; i<20; i++)
  {
    bits = 7;
    while (bits > -1)
    {      
      clockbyte = clockbyte ^ xorclock;
      if (clockbyte == 1)
        PORTD |= 0b00001000;                        // Set PD3 = HIGH
      else
        PORTD = PORTD & 0b11110111;                 // Set PD3 = LOW
      _delay_us(31);
      if (headerdata[i] & (1 << bits))
      {
        clockbyte = clockbyte ^ xorclock;
        if (clockbyte == 1)
        PORTD |= 0b00001000;                        // Set PD3 = HIGH
          else
        PORTD = PORTD & 0b11110111;                 // Set PD3 = LOW
      }
      _delay_us(39);
      bits--;
    }
  }
  //send last bit
  clockbyte = clockbyte ^ xorclock;
  if (clockbyte == 1)
    PORTD |= 0b00001000;                            // Set PD3 = HIGH
  else
    PORTD = PORTD & 0b11110111;                     // Set PD3 = LOW 
}

void SendDataASM()
{
  int bits = 7;

  //if (STOPPED)
  //  return;

  for (int i=0; i<1036; i++)
  {
    bits = 7;
    while (bits > -1)
    {      
      clockbyte = clockbyte ^ xorclock;
      if (clockbyte == 1)
        PORTD |= 0b00001000;                        // Set PD3 = HIGH
      else
        PORTD = PORTD & 0b11110111;                 // Set PD3 = LOW
      _delay_us(31);
      if (blockdata[i] & (1 << bits))
      {
        clockbyte = clockbyte ^ xorclock;
        if (clockbyte == 1)
        PORTD |= 0b00001000;                        // Set PD3 = HIGH
          else
        PORTD = PORTD & 0b11110111;                 // Set PD3 = LOW
      }
      _delay_us(39);
      bits--;
    }
  }
  //if (STOPPED)
  //  return; 
}

void SendCRCASM()
{
  int bits = 7;

  //if (STOPPED)
  //  return;

  for (int i=0; i<450; i++)
  {
    bits = 7;
    while (bits > -1)
    {      
      clockbyte = clockbyte ^ xorclock;
      if (clockbyte == 1)
        PORTD |= 0b00001000;                        // Set PD3 = HIGH
      else
        PORTD = PORTD & 0b11110111;                 // Set PD3 = LOW
      _delay_us(31);
      if (crcdata[i] & (1 << bits))
      {
        clockbyte = clockbyte ^ xorclock;
        if (clockbyte == 1)
        PORTD |= 0b00001000;                        // Set PD3 = HIGH
          else
        PORTD = PORTD & 0b11110111;                 // Set PD3 = LOW
      }
      _delay_us(39);
      bits--;
    }
  }
  PORTD = PORTD & 0b11110111;                       // Set PD3 = LOW 
}
