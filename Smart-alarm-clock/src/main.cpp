#include "Arduino.h"
#include "Audio.h"
#include "SD.h"
#include "Ticker.h"
#include "WiFiMulti.h"

Audio audio;          // 创建一个音频对象
WiFiMulti wifiMulti;  // 创建一个wifi对象

void Wifi(void);             // 声明一个wifi连接函数
void InitAudioPlayer(void);  // 声名一个音频播放函数
void SerialPortAudio(void);  // 声名一个串口音频函数

Ticker serialPortAudio = Ticker(SerialPortAudio, 1000, 0, MILLIS);

void setup() {
  Serial.begin(115200);  // 初始串口

  WiFi.mode(WIFI_STA);                   // 设置wifi模式为STA
  wifiMulti.addAP("4_503", "88888888");  // 添加wifi的SSID和密码
  Wifi();                                // 连接wifi

  InitAudioPlayer();  // 初始化音频播放器

  serialPortAudio.start();  // 启动定时器
}

void loop() {
  audio.loop();

  serialPortAudio.update();
}

void Wifi() {
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }

  // 当WiFi连接成功时，打印出"WiFi is Connect"
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("WiFi is Connect");
  }
}

void InitAudioPlayer() {
  audio.setPinout(4, 5, 6);  // 设置音频输出引脚
  audio.setVolume(8);        // 设置音量
  audio.connecttohost(
      "http://music.163.com/song/media/outer/url?id=432486474.mp3");  // 连接到音频源
}

void SerialPortAudio() {
  if (Serial.available()) {  // 将streamURL放入串行监视器中
    audio.stopSong();
    String r = Serial.readString();
    r.trim();
    if (r.length() > 5) audio.connecttohost(r.c_str());
    log_i("free heap=%i", ESP.getFreeHeap());
  }
}
