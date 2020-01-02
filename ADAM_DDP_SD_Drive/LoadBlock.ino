void LoadBlock(unsigned long blocknumber, unsigned int Index){                           // Load block from SD card to buffer array
  byte crcA = 0;
  byte crcB = 0;
  //byte tempB = 0;
  byte checksum = 0;
  //bool carry = false;
  //bool rcarry = false;
  byte currentbyte = 0x00;
  int holdcurrentbyte = 0;
  int holdcrcB = 0;
  //int phys_block = 0;
  //byte phys_block = 0;

  PreviousBlock = CurrentBlock;
  digitalWrite(STATUSLED,HIGH);// Turn on the status LED

  //Serial.print("Loading Block ");
  //Serial.println(blocknumber);

  file.open(sd.vwd(),Index, O_READ);

  //if (blocknumber > 127)
  //  blocknumber = blocknumber - 128;
  //calculate tape physical block
  //phys_block = 64 + CurrentBlock;
  //if (phys_block > 128)
  //  phys_block = 128 - phys_block;
  checksum = 0;
  
  headerdata[0] = 0x00;
  headerdata[1] = 0x00;
  headerdata[2] = 0x00;
  headerdata[3] = 0x00;
  headerdata[4] = 0x00;
  headerdata[5] = 0x00;
  headerdata[6] = 0x00;
  headerdata[7] = 0x00;
  headerdata[8] = 0x00;
  headerdata[9] = 0x00;
  headerdata[10] = 0x16;                //sync byte
  headerdata[11] = 0x47;                //header id  'G'
  headerdata[12] = 0x57;                //header id  'W'
  //headerdata[4] = 0x48;              //header id  'H'
  //headerdata[5] = 0x4E;              //header id  'E'
  headerdata[13] = 0x00;                //block
  headerdata[14] = blocknumber;         //block
  //headerdata[7] = trackmap[blocknumber];   //block
  //headerdata[7] = phys_block_byte;   //block
  headerdata[15] = 0xff;                //ones complement
  headerdata[16] = ~blocknumber;        //ones complement
  //headerdata[9] = ~trackmap[blocknumber];  //one complement blocknumber
  //headerdata[9] = ~phys_block_byte;  //one complement blocknumber
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
  
  //This routine calculates the CRC of the data in the lK buffer.
  //The algorithm used here calculates CRC16. The memory buffer is
  //looked at bit by bit . For each bit, we XOR it with the bottom
  //bit of the CRC register . The result is then XORed with bits
  //14 and 1 of the CRC register . Finally, the CRC register is
  //shifted right, with the calculated bit being shifted into the
  //top of the register.
  /*
  for (int i=17; i<=1040;i++){
      currentbyte = file.read();
      blockdata[i] = currentbyte;

      //calculate crc16
      for (int j = 0; j < 8; j++)
      {
        if (currentbyte & 0x80) { carry = true; } else { carry = false; }
        currentbyte = currentbyte << 1;                   //LSL
        if (carry) tempB += 1;                            //ADCB
        if(tempB & 0x01) { carry = true; } else { carry = false; } //LSRB
        if (carry == true)
        {
          crcA = crcA ^ 64;                               //EORA #01000000B
          crcB = crcB ^ 2;                                //EORB #00000010B
        }
        if (crcA & 0x01) { rcarry = true; } else { rcarry = false; }
        crcA = crcA >> 1;
        if (carry == true)
          bitSet(crcA,7);
        else
          bitClear(crcA,7);
        crcB = crcB >> 1;
        if (rcarry == true)
          bitSet(crcB,7);
        else
          bitClear(crcB,7);
        tempB = crcB;
      }
  }
  blockdata[1041] = crcA;
  blockdata[1042] = crcB;
  */
  file.close();
  digitalWrite(STATUSLED,LOW);// Turn off the status LED
  /*int counter = 0;
  for (int i=11; i<=1034; i++) {
    Serial.print(counter);
    Serial.print(" - ");
    Serial.println(blockdata[i],HEX);
    counter++;
  }*/
  //Serial.print("Block loaded = ");
  //Serial.println(CurrentBlock);
  //Serial.print("L: ");
  //Serial.println(CurrentBlock);
}
