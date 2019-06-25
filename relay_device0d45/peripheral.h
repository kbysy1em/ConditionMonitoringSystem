#ifndef T_SENSOR_UNIT
#define T_SENSOR_UNIT
#include "util.h"

extern SoftwareSerial mySerial;

// 応答IDの設定
typedef enum {
  NORMAL_ID = 80,  // 0x50
} ResponseId;

// メッセージの長さの設定
typedef enum {
  TX_HEADER_LEN = 9,     // 送信メッセージのヘッダー長
  RX_HEADER_LEN = 29,    // 受信メッセージのヘッダー長
  MEAS_VALUE_LEN = 4,    // 測定値の文字列長
  NUM_OF_MEAS_ITEMS = 1, // ■■測定値の個数
  MAX_CONTENT_LEN = 22,  // NUM_OF_MEAS_ITEMS*MEAS_VALUE_LEN+2
} MessageLength;

// LEDのピンの設定
typedef enum {
  SENSOR_PIN = 0,
  RX_PIN = 6,
  TX_PIN = 7,
  LED_PIN = 2,
} PinSetting;

// TWELITE論理IDの設定
typedef enum {
  THIS_TWELITE = 45,       // ■■このTWELITEの論理ID(10進数)
  DOWNSTREAM_TWELITE = 40, // ■■下流側TWELITEの論理ID(10進数)
  IGNORE_TWELITE = 219,    // 0xDB
} TweliteId;

// 受信メッセージを格納する構造体
typedef struct {
  const char *message;     // 受信した文字列
  unsigned int tweliteId;  // 送信元のTWELITEの論理ID
  unsigned int responseId; // 受信メッセージのヘッダーに含まれている応答ID
  int contentLength;       // HEADERとCHECKSUMを除いた文字列長
} RxContent;
#endif
