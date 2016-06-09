UserInterfaceObject::UserInterfaceObject()
{
  delay(100);
  _destroyed = true;
}

void UserInterfaceObject::begin()
{
  #ifdef PRINT_STATUS
  Serial.println(F("Launched User Interface"));
  #endif
  _destroyed = false;
}

UserInterfaceObject::~UserInterfaceObject()
{
  end();
}

void UserInterfaceObject::end()
{
  Serial.end();
  _destroyed = true;
}

