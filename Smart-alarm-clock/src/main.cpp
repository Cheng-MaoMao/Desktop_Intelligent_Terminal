#include "Arduino.h"
#include "Audio.h"
#include "SD.h"
#include "Ticker.h"
#include "WiFiMulti.h"

Audio audio;          // 创建一个音频对象
WiFiMulti wifiMulti;  // 创建一个wifi对象
int volume = 8;       // 设置音量

void Wifi(void);             // 声明一个wifi连接函数
void InitAudioPlayer(void);  // 声名一个音频播放函数
void SerialPortAudio(void);  // 声名一个串口音频和音量函数

Ticker serialPortAudio =
    Ticker(SerialPortAudio, 1000, 0,
           MILLIS);  // 创建一个串口接受音频URL和音量的定时器

void setup() {
  Serial.begin(115200);  // 初始串口

  Wifi();  // 连接wifi

  InitAudioPlayer();  // 初始化音频播放器

  serialPortAudio.start();  // 启动定时器
}

void loop() {
  audio.loop();

  serialPortAudio.update();
}

void Wifi() {
  WiFi.mode(WIFI_STA);                   // 设置wifi模式为STA
  wifiMulti.addAP("4_503", "88888888");  // 添加wifi的SSID和密码

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.println("WiFi连接中..");
  }

  // 当WiFi连接成功时，打印出"WiFi is Connect"
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("WiFi连接成功");
  }
}

void InitAudioPlayer() {
  audio.setPinout(4, 5, 6);  // 设置音频输出引脚
  audio.setVolume(volume);   // 设置音量
  audio.connecttohost(
      "http://music.163.com/song/media/outer/"
      "url?id=432486474.mp3");  // 连接到音频源
}

void SerialPortAudio() {
  if (Serial.available()) {
    String r = Serial.readString();
    r.trim();  // 去掉字符串前后的空格
    if (r.length() > 5 && (r.startsWith("http") || r.startsWith("https"))) {
      audio.stopSong();                         // 停止当前播放的音频
      audio.connecttohost(r.c_str());           // 连接到音频源
      log_i("剩余内存=%i", ESP.getFreeHeap());  // 打印剩余内存
    } else if (r.endsWith("#")) {
      volume = r.substring(0, r.length()).toInt();  // 获取音量
      audio.setVolume(volume);                      // 设置音量
    } else {
      Serial.println("URL格式错误");
    }
  }
}
