#include <SPI.h>
#include <TFT_eSPI.h>      // 引入硬件特定库
#include <Adafruit_ST7796.h> // 引入ST7796的库

TFT_eSPI tft = TFT_eSPI(); // 调用自定义库

void touch_calibrate(); // 声明触摸屏校准函数

void setup() {
  // 初始化TFT屏幕
  tft.init();

  // 在校准之前，将旋转设置为您希望���项目中使用的方向
  tft.setRotation(1);

  // 校准触摸屏并获取缩放因子
  touch_calibrate();

  // 清屏
  tft.fillScreen(TFT_BLACK);
  tft.drawCentreString("触摸屏幕进行测试！", tft.width() / 2, tft.height() / 2, 2);
}

void loop(void) {
  uint16_t x = 0, y = 0; // 用于存储触摸坐标

  // 如果屏幕被有效触摸，Pressed将被设置为true
  bool pressed = tft.getTouch(&x, &y);

  // 在检测到的坐标处画一个白色的点
  if (pressed) {
    tft.fillCircle(x, y, 2, TFT_WHITE);
  }
}

// 运行屏幕校准的代码，在setup()中设置了校准值时不需要
void touch_calibrate() {
  // 在这里编写您的校准代码
}
