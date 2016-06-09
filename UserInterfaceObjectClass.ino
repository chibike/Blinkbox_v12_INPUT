UserInterfaceObject::UserInterfaceObject( void )
{
  delay(100);
  _destroyed = true;
}

void UserInterfaceObject::begin()
{
  #ifdef PRINT_STATUS
  Serial.println("Launched User Interface");
  #endif
  _destroyed = false;
}

void UserInterfaceObject::end()
{
  Serial.end();
  _destroyed = true;
}
