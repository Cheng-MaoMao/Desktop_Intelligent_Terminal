#include "ReadSD.h"

File file = SD.open("/");  // 创建file对象=根目录

void initSd() {
  // 等待USB串口
  while (!Serial) {
    yield();
  }

  if (!SD.begin()) {
    Serial.println("初始化SD卡失败");
    return;
  } else {
    Serial.println("初始化SD卡成功");
  }
}

unsigned long getSDUsedSpace() {
  unsigned long usedSpace = 0;

  // 打开根目录
  if (file) {
    // 遍历根目录中的所有文件和目录
    while (true) {
      File entry = file.openNextFile();
      if (!entry) {
        // 没有更多的文件或目录
        break;
      }
      // 累加文件或目录的大小
      usedSpace += entry.size();
      entry.close();
    }
    file.close();
  }

  return usedSpace;
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // 如果没有文件则跳出循环
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // 文件会大小,文件夹则不打印大小
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}