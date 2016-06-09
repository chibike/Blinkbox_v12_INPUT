DistanceSensorObject::DistanceSensorObject( void )
{
  delay(100);
  _destroyed = true;
}

void DistanceSensorObject::begin()
{
  #ifdef PRINT_STATUS
  Serial.println("Launched Distance Sensor");
  #endif
  
  _destroyed = false;
}

void DistanceSensorObject::end()
{
  frontSensor.end();
  _destroyed = true;
}
