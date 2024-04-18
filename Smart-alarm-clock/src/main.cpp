#include <Arduino.h>
#include "Adafruit_SHT31.h"
#include "Ticker.h"
#include "WiFiMulti.h"
#include "Audio.h"

#define I2S_DOUT 19
#define I2S_BCLK 12
#define I2S_LRC 18

void SHT31Read();//SHT31温湿度传感器读取函数
void Init();//初始化函数

/*SHT31温湿度传感器实例化*/
TwoWire sht31Wire = TwoWire(0);//SHT31温度传感器使用的I2C总线
Adafruit_SHT31 sht31 = Adafruit_SHT31(&sht31Wire);
Ticker sht31Ticker(SHT31Read,1000,0,MILLIS);//定时器，每隔1s读取一次SHT31传感器数据

/*Audio音频实例化*/
Audio audio;

/*WiFi实例化*/
WiFiMulti wifiMulti;

void setup() {
    sht31Ticker.start();
}

void loop() {
    sht31Ticker.update();
    audio.loop();
}

void Init(){
    Serial.begin(115200);//串口初始化

    sht31Wire.begin(8,9);//SHT31温湿度传感器使用的I2C总线初始化

    audio.setPinout(I2S_BCLK,I2S_LRC,I2S_DOUT);//Audio初始化
    audio.setVolume(18);//设置音量
    audio.connecttohost("http://music.163.com/song/media/outer/url?id=22677570.mp3");

    WiFiClass::mode(WIFI_STA);//WiFi初始化
    wifiMulti.addAP("MiKU","88888888");//连接WiFi
    for(int i= 0;i < 4;i++){
        if(wifiMulti.run() != WL_CONNECTED){
            Serial.println("WiFi not connected!");
            WiFi.disconnect(true);
        }else{
            Serial.println("WiFi connected!");
            break;
        }
        delay(1000);
    }
}

void SHT31Read(){
    if (!sht31.begin()) {
        Serial.println("Couldn't find SHT31");
    }else{
        float t = sht31.readTemperature();
        float h = sht31.readHumidity();
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.print("°C\tHumidity: ");
        Serial.print(h);
        Serial.println("%");
    }
}
