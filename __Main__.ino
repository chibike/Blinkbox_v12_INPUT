#include <Arduino.h>

void loop()
{
  Serial.begin(9600);
  Blink_OS Blink_OS_v12;
  Blink_OS_v12.begin();
  //while( !Blink_OS_v12.Ui.verifyUser("pi", "raspbian") ){}
  //Serial.println(F("pi@root/>"));
  while(1)
  {
    if ( Blink_OS_v12.Sensors.RfidSensor.cardAvailable() == true )
    {
      char buffer[20];
      Blink_OS_v12.Sensors.RfidSensor.getTagId(buffer);
      Serial.println((String)buffer);
    }
    else
    {
      Serial.println( "Place card in front of reader" );
    }
    delay(1000);
  }
}

void fd( uint8_t power, uint16_t distance )
{
  multivariate variables[2];
  uint8_t command = FWD_DIST;
  multivariate variable;
  variable.data._ui8 = 200;
  variable.datatype = isUi8;
  variables[0] = variable;

  variable.data._ui16 = 50;
  variable.datatype = isUi16;
  variables[1] = variable;

  
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

void setup()
{
}
