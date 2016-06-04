#include <Wire.h>

class UltrasonicSensor
{
  public:
    uint16_t getDistance();
    UltrasonicSensor();
    void begin( uint8_t trigPin, uint8_t echoPin );
    ~UltrasonicSensor();
    void end();
  private:
    bool _destroyed;
    uint8_t _trigPin;
    uint8_t _echoPin;
    float _microseconds2mm( float duration );
};

UltrasonicSensor::UltrasonicSensor()
{
  _destroyed = true;
}

void UltrasonicSensor::begin( uint8_t trigPin, uint8_t echoPin )
{
  _trigPin, _echoPin = trigPin, echoPin;
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, OUTPUT);
  
  _destroyed = false;
}

UltrasonicSensor::~UltrasonicSensor()
{
  _destroyed = true;
}

void UltrasonicSensor::end()
{
  _destroyed = true;
}

uint16_t UltrasonicSensor::getDistance()
{
  if (_destroyed == true){return 0;}
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(_trigPin, LOW);

  float duration = pulseIn( _echoPin, HIGH );
  duration /= 2;

  return (int)_microseconds2mm(duration);
}

float UltrasonicSensor::_microseconds2mm( float duration )
{// sound_speed = 2.94117647 um/mm
  return duration/2.94117647;
}

class DistanceSensorObject
{
  public:
    UltrasonicSensor frontSensor;
    DistanceSensorObject();
    void begin();
    ~DistanceSensorObject();
    void end();
  private:
    bool _destroyed;
};

DistanceSensorObject::DistanceSensorObject()
{
  _destroyed = false;
}

void DistanceSensorObject::begin()
{
  _destroyed = false;
}

DistanceSensorObject::~DistanceSensorObject()
{
  _destroyed = true;
}

void DistanceSensorObject::end()
{
  _destroyed = true;
}

class RfidSensorObject
{
  #define NO_CARD_DETECTED   0x00
  #define WIRE_NOT_AVAILABLE 0x01
  #define COLLISION_DETECTED 0x02
  #define NO_TAG             0x03
  #define UNEXPECTED_RESULT  0x04
  
  public:
    bool getTagId( char* buffer );
    bool compareTag( char* buffer, char* tag );
    bool cardAvailable();
    
    RfidSensorObject();
    void begin( uint8_t cardDetectPin );
    ~RfidSensorObject();
    void end();
  private:
    bool _destroyed;
    uint8_t _cardDetectPin;
    byte _address;
    byte _errorCode;
    void _i2c_wait_timeout( int timeout );
    bool _loadTag(char* buffer);
};

RfidSensorObject::RfidSensorObject()
{
  _destroyed = true;
}

void RfidSensorObject::begin( uint8_t cardDetectPin )
{
  _address = 0xA0;
  _cardDetectPin = cardDetectPin;
  pinMode(_cardDetectPin, INPUT_PULLUP);
  
  _destroyed = false;
}

RfidSensorObject::~RfidSensorObject()
{

  _destroyed = true;
}

void RfidSensorObject::end()
{
  _destroyed = true;
}

void RfidSensorObject::_i2c_wait_timeout( int timeout )
{
  long startTime = millis();
  while(Wire.available() <= 0 && (millis()-startTime) < timeout)
  {
    //wait
  }
}

bool RfidSensorObject::compareTag( char* buffer, char* tag )
{
  if( !getTagId(buffer) == true );
  {
    return false;
  }
  return strcmp(buffer, tag);
}

bool RfidSensorObject::getTagId( char* buffer )
{
  if(_destroyed = true){return false;}

  while ( !_loadTag(buffer) )
  {
    switch(_errorCode)
    {
      case NO_CARD_DETECTED:
        //Serial.println("No Card Detected");
        return false;
        break;
      case WIRE_NOT_AVAILABLE:
        //Serial.println("Wire Not Available");
        break;
      case COLLISION_DETECTED:
        //Serial.println("Collision Detected");
        break;
      case NO_TAG:
        //Serial.println("No Tag");
        return false;
        break;
      default:
        //Serial.println("Unexpected Result");
        return false;
    }
  }
}

bool RfidSensorObject::cardAvailable()
{
  return !digitalRead(_cardDetectPin);
}

bool RfidSensorObject::_loadTag( char* buffer )
{
  if ( !cardAvailable() )
  {
    _errorCode = NO_CARD_DETECTED;
    return false;
  }
  
  Wire.beginTransmission(_address);
  Wire.write(1);
  Wire.write(1);
  Wire.endTransmission();
  
  int index = 0;
  delay(5);
  Wire.requestFrom(_address, 11);
  _i2c_wait_timeout(5);
  if( Wire.available() )
  {
    byte len = Wire.read();
    while( Wire.available() < len )
    {//wait for all data to arrive
      if( !cardAvailable() )
      {
        _errorCode = NO_CARD_DETECTED;
        return false;
      }
    }

    byte data = Wire.read();//read command
    if(data != 1)
    {
      _errorCode = UNEXPECTED_RESULT;
      return false;
    }

    data = Wire.read();//read status
    switch(data)
    {
      case 0://Operation successfull
        len = len - 2;
        while(--len)
        {
          data = Wire.read();//read tag character by character
          if(data < 0x10)
          {
            buffer[index++] = '0';
          }
          buffer[index++] = (data >> 4) > 0x09 ? 
                             char( (data >> 4) + 55 ):char( (data >> 4) + 48 );
          buffer[index++] = (data & 0x0f) > 0x09 ? 
                             char( (data & 0x0f) + 55):char( (data & 0x0f) + 48 );
        }
        return true;
        break;
      case 0x0A:
        _errorCode = COLLISION_DETECTED;
        return false;
        break;
      case 1:
        _errorCode = NO_TAG;
        return false;
        break;
      default:
        _errorCode = UNEXPECTED_RESULT;
        return false;
        break;
    }
  }
  _errorCode = WIRE_NOT_AVAILABLE;
  return false;
}

class CompassSensorObject
{
  public:
    float getHeading();
    float getTargetDeviation();
    void setTargetHeading( int heading );
    
    CompassSensorObject();
    void begin();
    ~CompassSensorObject();
    void end();
  private:
    bool _destroyed;
    byte _address;
    int _targetHeading;
    void _i2c_wait_timeout( int timeout );   
};

void CompassSensorObject::_i2c_wait_timeout( int timeout)
{
  long startTime = millis();
  while(Wire.available() <= 0 && (millis()-startTime) < timeout)
  {
    //wait
  }
}

float CompassSensorObject::getHeading()
{
  if ( _destroyed == true ){return 0;}
  Wire.beginTransmission(_address);
  Wire.write(2);
  Wire.endTransmission();

  Wire.requestFrom(_address, 2);
  _i2c_wait_timeout(100);

  if (Wire.available() > 0)
  {
    float angle = ( (int)Wire.read() << 8 ) | ( (int)Wire.read() & 0x0f );
    return 360 - angle/10.0;
  }
  else
  {
    return 0;
  }
}

float CompassSensorObject::getTargetDeviation()
{
  if ( _destroyed == true ){return 0;}
  float trueHeading = getHeading();
  switch( round(trueHeading) )
  {
    case 0:
    case 360:
      trueHeading = 0;
      break;
    default:
      if (trueHeading > 180)
      {
        trueHeading = -1*(360 - trueHeading);
      }
  }

  //Centralize Heading about target Angle
  return trueHeading - _targetHeading;  
}

void CompassSensorObject::setTargetHeading(int heading)
{
  if ( _destroyed == true ){return;}
  switch(heading)
  {
    case 0:
    case 360:
    case -360:
      heading = 0;
      break;
    default:
      if (heading > 180)
      {
        heading = -1*(360 - heading);
      }
  }
  _targetHeading = heading;
}

CompassSensorObject::CompassSensorObject()
{
  _destroyed = true;
}

void CompassSensorObject::begin()
{
  _address = 0x60;
  _targetHeading = 0;
  
  _destroyed = false;
}

CompassSensorObject::~CompassSensorObject()
{
  _destroyed = true;
}

void CompassSensorObject::end()
{
  _destroyed = true;
}

class SensorObjects
{
  public:
    DistanceSensorObject DistanceSensor;
    RfidSensorObject RfidSensor;
    CompassSensorObject CompassSensor;
    
    SensorObjects();
    void begin();
    ~SensorObjects();
    void end();
  private:
    bool _destroyed;
};

SensorObjects::SensorObjects()
{
  _destroyed = true;
}

void SensorObjects::begin()
{
  _destroyed = false;
}

SensorObjects::~SensorObjects()
{
  _destroyed = true;
}

void SensorObjects::end()
{
  _destroyed = true;
}

void setup()
{
  Wire.begin();
}

void loop()
{
  //pass
}

