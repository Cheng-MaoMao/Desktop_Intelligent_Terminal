#ifndef GETSENSOR_H
#define GETSENSOR_H

#include "Adafruit_SHT31.h"
#include "Wire.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();  // 创建 SHT31 对象
boolean enableHeater = false;             // 定义加热器启用状态变量

struct TempHum {
  float temperature;
  float humidity;
};

TempHum ObtainTemperatureAndHumidity(void);  // 获取温湿度

#endif  // GETSENSOR_H