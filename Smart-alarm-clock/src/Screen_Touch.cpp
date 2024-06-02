#include "Screen_Touch.h"

void InitTouchParam() {
  ft6336u.begin();

  Serial.print("FT6336U固件版本: ");
  Serial.println(ft6336u.read_firmware_id());
  Serial.print("FT6336U设备模式: ");
  Serial.println(ft6336u.read_device_mode());
}

void ReadTouchParam() {
  if (digitalRead(INT_N_PIN) != -1) {
    Serial.print("FT6336U TD状态: ");
    Serial.println(ft6336u.read_td_status());
    Serial.print("FT6336U触摸事件/ID 1: (");
    Serial.print(ft6336u.read_touch1_event());
    Serial.print(" / ");
    Serial.print(ft6336u.read_touch1_id());
    Serial.println(")");
    Serial.print("FT6336U触摸位置1: (");
    Serial.print(ft6336u.read_touch1_x());
    Serial.print(" , ");
    Serial.print(ft6336u.read_touch1_y());
    Serial.println(")");
    Serial.print("FT6336U触控重量/MISC 1: (");
    Serial.print(ft6336u.read_touch1_weight());
    Serial.print(" / ");
    Serial.print(ft6336u.read_touch1_misc());
    Serial.println(")");
    Serial.print("FT6336U触摸事件/ID 2: (");
    Serial.print(ft6336u.read_touch2_event());
    Serial.print(" / ");
    Serial.print(ft6336u.read_touch2_id());
    Serial.println(")");
    Serial.print("FT6336U触摸位置2:(");
    Serial.print(ft6336u.read_touch2_x());
    Serial.print(" , ");
    Serial.print(ft6336u.read_touch2_y());
    Serial.println(")");
    Serial.print("FT6336U触控重量/MISC 2: (");
    Serial.print(ft6336u.read_touch2_weight());
    Serial.print(" / ");
    Serial.print(ft6336u.read_touch2_misc());
    Serial.println(")");
  }
}
