void setup()
{
}

void loop()
{
  Serial.begin(9600);
  Blink_OS Blink_OS_v12;
  Blink_OS_v12.begin();
  while(1)
  {
    Serial.println("In Loop");
    delay(1000); 
  }
}


void REQUEST_EVENT()
{
  //pass
}

void RECEIVE_EVENT()
{
  //pass
}

ISR(TIMER2_COMPA_vect)
{
  //pass
}

void shutdown()
{
  while(1)
  {
    #ifdef PRINT_STATUS
    Serial.println("Shutdown");
    #endif
  }
}
