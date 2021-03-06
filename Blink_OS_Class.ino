Blink_OS::Blink_OS( void )
{
  _destroyed = true;
  delay(100);
}
  
void Blink_OS::begin( void )
{
  Wire.begin(7);
  
  Sensors.begin();
  Sensors.DistanceSensor.begin();
  Sensors.DistanceSensor.frontSensor.begin( A0, 3 );//uint8_t trigPin, uint8_t echoPin
  Sensors.RfidSensor.begin( 2 ); //cardDetectPin
  
#ifdef ALLOW_ACCESS_2_COMPASS
  Sensors.CompassSensor.begin();
#endif
  
  IntraSystemCalls.begin();
  FileExplorer.begin( 10 );//cs pin
  
  #ifdef PRINT_STATUS
  Serial.println(F("Blinkbox OS v12 [Version 0.1]"));
  Serial.println(F("(c) 2015 Blink Corporation. All rights reserved."));
  #endif
  
  _destroyed = false;
}

void Blink_OS::end( void )
{
  Sensors.end();
  IntraSystemCalls.end();
  Ui.end();
  FileExplorer.end();
  _destroyed = true;
  shutdown();
}
