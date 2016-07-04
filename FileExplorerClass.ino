FileExplorerObject::FileExplorerObject( void )
{
  delay(100);
  _destroyed = true;
}

void FileExplorerObject::begin( uint8_t chipSelect )
{
  pinMode(chipSelect, OUTPUT);
  digitalWrite(chipSelect, HIGH);
  if(SD.begin( chipSelect ))
  {
    #ifdef PRINT_STATUS
    Serial.println("Opening \"/SysFiles.txt\"...");
    #endif
    File sysDir = SD.open("/SysFiles.txt", FILE_READ);
    if(sysDir)
    {
      #ifdef PRINT_STATUS
      Serial.println("Opened");
      Serial.println("Loading SysFiles...");
      #endif
      while(sysDir.available())
      {
        String buffer = sysDir.readStringUntil(';');
        sysDir.find('\n');
        #ifdef PRINT_STATUS
        Serial.print(buffer);
        Serial.println(F(" Loaded"));
        #else
        Serial.println(buffer);
        #endif
      }
    }
    else
    {
      Serial.println(F("Error Opening SysFiles.txt"));
    }
    sysDir.close();
  }
  else
  {
    #ifdef PRINT_STATUS
    Serial.println(F("Could not access drive"));
    #endif
  }
  _destroyed = false;
}

void FileExplorerObject::end()
{
  _destroyed = true;
}
