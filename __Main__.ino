//IntraSystemCallsObject IntraSystemCalls;
//FileExplorerObject FileExplorer;
//SensorObjects Sensors;
//UserInterfaceObject Ui;
//UltrasonicSensor frontSensor;

void setup()
{
  pinMode(13, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("In Setup");
}

void loop()
{
  Serial.println("In Loop");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}
