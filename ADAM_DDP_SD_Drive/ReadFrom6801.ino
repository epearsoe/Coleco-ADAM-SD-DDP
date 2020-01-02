void ReadFrom6801()
{
int ByteCount = 0;
int BitCount = 8;
byte IncomingByte = 0;
int Status = 0;
int LastSeen = 0;
byte currentbyte = 0;
int holdcurrentbyte = 0;
int holdcrcB = 0;
byte crcB = 0;
byte crcA = 0;

while (ByteCount < 1034)
  {
    while(BitCount > 0)
    {
      Status = PIND & _BV(PD2);
      while ((LastSeen = PIND & _BV(PD2)) == Status);
      _delay_us(32);
      Status = PIND & _BV(PD2);
      IncomingByte <<=1;
      if (Status != LastSeen)
        IncomingByte |= 0x01;
      writedata[ByteCount] = IncomingByte;
      BitCount--;
    }
    BitCount = 8;
    ByteCount++;
  }
  //This routine calculates the sum of the data in the 1K buffer.
  /*for (int i=5; i<=1028; i++){
    currentbyte = writedata[i];
    holdcurrentbyte = currentbyte;
    holdcrcB = crcB;
    crcB = crcB + currentbyte;
    holdcrcB = holdcrcB + holdcurrentbyte;
    if (holdcrcB > 255)
      crcA += 1;
  }
  if (writedata[1032] != crcA || writedata[1033] != crcB) //crcfail
  {
    Serial.println("CRC fail");
  }*/
  
  SaveBlock(CurrentBlock, ddpfileIndex);
  
  return;
}
