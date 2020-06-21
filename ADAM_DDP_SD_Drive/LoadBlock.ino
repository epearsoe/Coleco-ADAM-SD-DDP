void LoadBlock(unsigned long blocknumber, unsigned int Index){                           // Load block from SD card to buffer array
  byte crcA = 0;
  byte crcB = 0;
  byte checksum = 0;
  byte currentbyte = 0x00;
  int holdcurrentbyte = 0;
  int holdcrcB = 0;
  unsigned long sendblocknumber = 0;
  
  sendblocknumber = blocknumber;

  if (!TAPEGWMODE)
  {
    if (blocknumber < 64)
      blocknumber = 64 + blocknumber;
    else if (blocknumber > 63 && blocknumber < 127)
      blocknumber = blocknumber - 64;
    headerdata[11] = 0x48;              //header id  'H'
    headerdata[12] = 0x45;              //header id  'E'
  }
  else
  {
    headerdata[11] = 0x47;                //header id  'G'
    headerdata[12] = 0x57;                //header id  'W'
  }

  //Serial.println(blocknumber);
  
  PreviousBlock = CurrentBlock;
  
  digitalWrite(STATUSLED,HIGH);// Turn on the status LED

  file.open(sd.vwd(),Index, O_READ);

  if (sendblocknumber > 127)
    sendblocknumber = sendblocknumber - 128;
 
  checksum = 0;

  for (int i=0; i<10; i++)
    headerdata[i] = 0x00;
  headerdata[10] = 0x16;                //sync byte
  headerdata[13] = 0x00;                //block
  headerdata[14] = sendblocknumber;         //block
  headerdata[15] = 0xff;                //ones complement
  headerdata[16] = ~sendblocknumber;        //ones complement
  headerdata[17] = 0x00;               //max block hi
  headerdata[18] = 0x80;               //max block lo

  for (int i=11; i<19; i++)
    checksum = checksum + headerdata[i];
  headerdata[19] = ~checksum;          //ones complement sum of above

  //format 1k blockdata array
  for (int i=0; i<1040; i++)
    blockdata[i] = 0x00;
  
  blockdata[10] = 0x16;               //sync byte
  unsigned long blocklocation = blocknumber*0x00000400;
  file.seekSet(blocklocation);

  //This routine loads 1K and calculates the sum of the data.
  for (int i=11; i<=1034; i++){
    currentbyte = file.read();
    blockdata[i] = currentbyte;
    holdcurrentbyte = currentbyte;
    holdcrcB = crcB;
    crcB = crcB + currentbyte;
    holdcrcB = holdcrcB + holdcurrentbyte;
    if (holdcrcB > 255)
      crcA += 1;
  }
  
  //format crcdata array
  for (int i=0; i<500; i++)
    crcdata[i] = 0x00;
  
  crcdata[20] = 0x16;          //sync byte
  crcdata[21] = crcA;          //sum of data
  crcdata[22] = crcB;          //sum of data
  
  file.close();
  digitalWrite(STATUSLED,LOW);// Turn off the status LED
}
