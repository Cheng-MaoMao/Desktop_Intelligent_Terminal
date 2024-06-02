#include "GetSensor.h"

TempHum ObtainTemperatureAndHumidity() {
  TempHum result;  // 定义结构体变量保存温湿度

  /*检测SHT31是否找到*/
  if (!sht31.begin(0x44)) {
    Serial.println("未找到 SHT31 传感器");
    while (1) {
      yield();
    }
  } else {
    sht31.heater(false);  // 关闭加热器
  }

  /*检测加热器是否启用*/
  Serial.print("加热器启用状态: ");
  enableHeater = sht31.isHeaterEnabled();
  if (enableHeater) {
    Serial.println("启用");
  } else {
    Serial.println("未启用");
  }

  /*读取温湿度*/
  float tem = sht31.readTemperature();  // 读取温度
  float hum = sht31.readHumidity();     // 读取湿度
  String str;                           // 定义字符串保存温湿度

  if (!isnan(tem) && !isnan(hum)) {
    str = "温度: " + String(tem) + " ℃, 湿度: " + String(hum) + " %";
    Serial.println(str);
  }

  result.temperature = sht31.readTemperature();  // 读取温度
  result.humidity = sht31.readHumidity();        // 读取湿度

  return result;
}
