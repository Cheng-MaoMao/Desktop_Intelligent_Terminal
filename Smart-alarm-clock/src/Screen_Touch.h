#ifndef SCREEN_TOUCH_H
#define SCREEN_TOUCH_H

#include "FT6336U.h"  //引用触摸屏库

#define I2C_SDA 22
#define I2C_SCL 23
#define RST_N_PIN 21
#define INT_N_PIN 34

FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN);  // 初始化触摸屏

void InitTouchParam(void);  // 初始化触摸屏参数
void ReadTouchParam(void);  // 读取触摸屏参数
void TouchClick(void);      // 触摸屏点击事件

#endif