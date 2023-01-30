#include <Arduino.h>
#include "AwotFirmwareCore.h"

void setup()
{
  Serial.begin(115200);
  AwotFirmwareCore::Begin();
  // put your setup code here, to run once:
}

void loop()
{
  AwotFirmwareCore::Loop();
}