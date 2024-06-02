#ifndef READSD_H
#define READSD_H

#include "Arduino.h"
#include "SD.h"
#include "SPI.h"

void initSd(void);                    // 初始化SD卡
unsigned long getSDUsedSpace(void);  // 获取SD卡已使用空间
void printDirectory(File, int);      // 打印SD卡目录

#endif  // READSD_H