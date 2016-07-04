IntraSystemCallsObject::IntraSystemCallsObject( void )
{
  delay(100);
  _destroyed = true;
  _address = 8;
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


void IntraSystemCallsObject::sendCommand( uint8_t command, multivariate *variables, uint16_t var_len )
{
  Wire.beginTransmission(_address);
  Wire.write(command);
  
  for (int i = 0; i < var_len; i++)
  {
    switch ( variables[i].datatype )
    {
      case 0:
        Wire.write( variables[i].data._b );
        break;
      case 1:
        Wire.write( (((int)variables[i].data._f) >> 8) & 0x00ff, ((int)variables[i].data._f) & 0x00ff );
        break;
      case 2:
        Wire.write( (((int)variables[i].data._d) >> 8) & 0x00ff, ((int)variables[i].data._d) & 0x00ff );
        break;
      case 3:
        Wire.write( variables[i].data._ui8 );
        break;
      case 4:
        Wire.write( (((variables[i].data._ui16) >> 8) & 0x00ff, (variables[i].data._ui16) & 0x00ff );
        break;
      case 5:
        Wire.write( variables[i].data._i8 );
        break;
      case 6:
        Wire.write( ((variables[i].data._i16) >> 8) & 0x00ff, ((variables[i].data._i16) & 0x00ff );
        break;
      default:
        continue;
    }
  }
  
  Wire.endTransmission();
}

