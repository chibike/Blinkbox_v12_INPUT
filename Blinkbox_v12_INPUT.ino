#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#define PRINT_STATUS

#define isBoolean 0
#define isFloat   1
#define isDouble  2
#define isUi8     3
#define isUi16    4
#define isI8      5
#define isI16     6

#define STOP_CMD    0x00
#define FWD         0x01
#define FWD_DIST    0x02
#define BWD         0x03
#define BWD_DIST    0x04
#define STB         0x05
#define ST_HD       0x06
#define ST_PWR      0x08
#define LT_IND      0x09
#define RT_IND      0x10
#define FL_AL       0x11
#define HDL         0x12
#define FL_HDL      0x13
#define FL_HRN      0x14
#define PAUSE       0x15
#define PLAY        0x16
#define FOLLOW_LINE 0x17
#define TRN_FWD     0x20
#define TRN_BWD     0x21
#define AVD_OBS     0x23
#define NONE_CMD    0x50

typedef union
{
    bool _b;
    float _f;
    double _d;
    uint8_t _ui8;
    uint16_t _ui16;
    int8_t _i8;
    int16_t _i16;
} Data;

typedef struct
{
    uint8_t datatype;
    Data data;
} multivariate;



class UltrasonicSensor
{
  public:
    UltrasonicSensor();
    uint16_t getDistance();
    void begin( uint8_t trigPin, uint8_t echoPin );
    void end();
  private:
    bool _destroyed;
    uint8_t _trigPin;
    uint8_t _echoPin;
    float _microseconds2mm( float duration );
};


class DistanceSensorObject
{
  public:
    UltrasonicSensor frontSensor;
    
    DistanceSensorObject();
    void begin();
    void end();
  private:
    bool _destroyed;
};

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
    void end();
  private:
    bool _destroyed;
    uint8_t _cardDetectPin;
    byte _address;
    byte _errorCode;
    void _i2c_wait_timeout( int timeout );
    bool _loadTag(char* buffer);
};


#ifdef ALLOW_ACCESS_2_COMPASS

class CompassSensorObject
{
  public:
    float getHeading();
    float getTargetDeviation();
    void setTargetHeading( int heading );
    
    CompassSensorObject();
    void begin();
    void end();
  private:
    bool _destroyed;
    byte _address;
    int _targetHeading;
    void _i2c_wait_timeout( int timeout );   
};

#endif

class SensorObjects
{
  public:
    DistanceSensorObject DistanceSensor;
    RfidSensorObject RfidSensor;

    SensorObjects();
    void begin();
    void end();
#ifdef ALLOW_ACCESS_2_COMPASS
    CompassSensorObject CompassSensor;
#endif
  private:
    bool _destroyed;
};

class IntraSystemCallsObject
{
  public:
    void sendCommand( uint8_t command, multivariate *variables, uint16_t var_len );
    
    IntraSystemCallsObject();
    void begin();
    void end();
  private:
    bool _destroyed;
    byte _address;
};


class FileExplorerObject
{
  public:
    SDClass drive;
    File openRoot()
    {
      return drive.open("/");
    }

    void getDir(char* name, char* buffer)
    {
      File dir = openRoot();
      while(1)
      {
        File entry = dir.openNextFile();
        strcat(buffer, "/");
        if(!entry)
        {
          strcat(buffer, entry.name());
          return;
        }
        else if( strcmp(entry.name(), name) )
        {
          strcat(buffer, entry.name());
          return;
        }
        else if( entry.isDirectory() )
        {
          dir = search(entry, name);
          if( strcmp( dir.name(), name ) )
          {
            strcat(buffer, dir.name());
            return;
          }
        }
      }
    }

    File search(File dir, char* name)
    {
      while(1)
      {
        File entry = dir.openNextFile();
        if(!entry)
        {
          return entry;
        }
        else if( strcmp(entry.name(), name) )
        {
          return entry;
        }
        else if( entry.isDirectory() )
        {
          dir = search(entry, name);
          if( strcmp( dir.name(), name ) )
          {
            return dir;
          }
        }
      }
    }
    FileExplorerObject();
    void begin( uint8_t chipSelectPin );
    void end();
  private:
    bool _destroyed;
    SdVolume volume;
    SdFile root;
};

class UserInterfaceObject
{
  public:
    bool verifyUser( char* ssid, char* key )
    {
      Serial.println("Enter Username>");
      while( !Serial.available() ){}
      if(String(ssid) != Serial.readString())
      {
        return false;
      }
      else
      {
        Serial.println("Enter Password>");
        while( !Serial.available() ){}
        if(String(key) == Serial.readString())
        {
          return true;
        }
        else
        {
          return false;
        }
      }
    }
    void input( char* buffer, uint8_t n, char* prompt )
    {
      Serial.print(prompt);
      while( !Serial.available() ){}//wait for response
      
      for (uint8_t i=0; i<Serial.available(); i++)
      {
        if(i >= n){Serial.flush();return;}
        buffer[i] = Serial.read();
      }
    }
    void input( char* buffer, uint8_t n, char* prompt, long timeout )
    {
      Serial.print(prompt);
      long startTime = millis();
      while(!Serial.available() && millis()-startTime < timeout){}//wait for response
      
      for (uint8_t i=0; i<Serial.available(); i++)
      {
        if(i >= n){Serial.flush();return;}
        buffer[i] = Serial.read();
      }
    }
    
    float inputFloat( char* prompt, long timeout )
    {
      Serial.print(prompt);
      long startTime = millis();
      while(!Serial.available() && millis()-startTime < timeout)//wait for response
      return Serial.parseFloat();
    }
    
    int inputInt( char* prompt, long timeout )
    {
      Serial.print(prompt);
      long startTime = millis();
      while(!Serial.available() && millis()-startTime < timeout)//wait for response
      return Serial.parseInt();
    }
    UserInterfaceObject();
    void begin();
    void end();
  private:
    bool _destroyed;
};

class Blink_OS
{
  public:
    UserInterfaceObject Ui;
    IntraSystemCallsObject IntraSystemCalls;
    FileExplorerObject FileExplorer;
    SensorObjects Sensors;
    
    Blink_OS();
    void begin();
    void end();
  private:
    boolean _destroyed;
};

