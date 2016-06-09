IntraSystemCallsObject::IntraSystemCallsObject()
{
  delay(100);
  _destroyed = true;
}

void IntraSystemCallsObject::begin()
{
  _destroyed = false;

  #ifdef PRINT_STATUS
  Serial.println(F("Launched Intra Sys Calls"));
  #endif
}

IntraSystemCallsObject::~IntraSystemCallsObject()
{
  end();
}

void IntraSystemCallsObject::end()
{
  _destroyed = true;
}

