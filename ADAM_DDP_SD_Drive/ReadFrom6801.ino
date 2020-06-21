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
      blockdata[ByteCount] = IncomingByte;
      BitCount--;
    }
    BitCount = 8;
    ByteCount++;
  }
  
  SaveBlock(CurrentBlock, ddpfileIndex);
  
  return;
}
