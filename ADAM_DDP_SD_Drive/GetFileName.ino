String GetFileName(unsigned int IncomingFileIndex){                                                // Get the FileName from Index Number
  char incomingfilename[255];
  if (IncomingFileIndex == 0){
    return " ";
  }
  sd.vwd()->seekSet(IncomingFileIndex);
  file.open(sd.vwd(),IncomingFileIndex, O_READ);
  file.getName(incomingfilename,255);
  String testfilename = String(incomingfilename);
  testfilename.trim();
  testfilename = testfilename.substring(0,namelength);  // Trim to the size of the maximum global namelength
  file.close();
  return testfilename;
}
