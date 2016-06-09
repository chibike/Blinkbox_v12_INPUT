DistanceSensorObject::DistanceSensorObject()
{
  delay(100);
  _destroyed = true;
}

void DistanceSensorObject::begin()
{
  #ifdef PRINT_STATUS
  Serial.println(F("Launched Distance Sensor"));
  #endif
  
  _destroyed = false;
}

DistanceSensorObject::~DistanceSensorObject()
{
  end();
}

void DistanceSensorObject::end()
{
  frontSensor.end();
  _destroyed = true;
}

