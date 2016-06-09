IntraSystemCallsObject::IntraSystemCallsObject( void )
{
  delay(100);
  _destroyed = true;
}

void IntraSystemCallsObject::begin()
{
  _destroyed = false;

  #ifdef PRINT_STATUS
  Serial.println("Launched Intra Sys Calls");
  #endif
}

void IntraSystemCallsObject::end()
{
  _destroyed = true;
}
