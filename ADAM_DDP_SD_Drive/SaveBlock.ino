int SaveBlock(unsigned long blocknumber, unsigned int Index){                // Save the buffer array to the SD card
  
  if (blocknumber < 64)
    blocknumber = 64 + blocknumber;
  else if (blocknumber > 63 && blocknumber < 127)
    blocknumber = blocknumber - 64;
    
  unsigned long blocklocation = blocknumber*0x00000400;
  
  digitalWrite2f(STATUSLEDpin,HIGH);// Turn on the status LED

  file.open(sd.vwd(),Index, O_READ | O_WRITE);

  file.seekSet(blocklocation);
  
  for (int i=5; i<=1028;i++){
        file.write(writedata[i]);
  }
  
  file.close();
  
  digitalWrite2f(STATUSLEDpin,LOW);// Turn off the status LED 
  Serial.print("Block saved = ");
  Serial.println(blocknumber); 
  return 1;
}
