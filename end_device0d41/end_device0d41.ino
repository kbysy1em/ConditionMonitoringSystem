#include <SoftwareSerial.h>
#include "peripheral.h"
#include "handler.h"

static const unsigned long MeasInterval = 3000;  // ■■測定間隔
SoftwareSerial mySerial(RX_PIN, TX_PIN);
static unsigned long previousMillis = 0;
static char rxMessage[RX_HEADER_LEN + MAX_CONTENT_LEN + 3] = "";
static int rxMsgLength;

void acquire(int *measValue);
void transmit(const int *measValue);
bool parseMessage(MessageHandler *pHandler, const char *rxMessage);

RxMsgHandler ignoreHandler = {{handleIgnore}, 0, 0};
RxMsgHandler normalMsgHandler = {{handleNormalMsg}, NORMAL_ID, MAX_CONTENT_LEN};
ChainedHandler normalMsgChain = {{isHandled}, &normalMsgHandler.base, NULL};
ChainedHandler ignoreChain = {{isHandled}, &ignoreHandler.base, &normalMsgChain.base};

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) yield();
  mySerial.begin(9600);
}

void loop() {
  // 一定時間おきにデータを測定して，送信するための処理
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= MeasInterval) {
    int measValue[NUM_OF_MEAS_ITEMS];
    acquire(measValue);
    transmit(measValue);
    previousMillis = currentMillis;
  }

  //データを受信した場合の処理
  while (mySerial.available()) {
    char c = mySerial.read();
    
    // 受信した文字が改行コードLFの場合には受信した文字列を解析する
    // LFとCR以外の場合には受信文字をrxMessageに蓄積する
    if (c == 10) {
      rxMessage[rxMsgLength] = '\0';
      Serial.print("RX: ");
      Serial.println(rxMessage);
      if (!parseMessage(&ignoreChain.base, rxMessage))
        Serial.println("Handler: None");
      rxMessage[0] = '\0';
      rxMsgLength = 0;
    } else if (c != 13) {
      rxMessage[rxMsgLength] = c;
      rxMsgLength++;
    }
  }
}

// ■■測定値を取得する．戻り値は整数とする
void acquire(int *measValue) {
  int rawValue = analogRead(SENSOR_PIN); 
  long volt = map(rawValue, 0, 1023, 0, 3300);
  measValue[0] = ((float)volt - 600.0) / 10.0; 
}

// 測定値を文字列としてTWELITEへ送信する
void transmit(const int *measValue) {
  char txMessage[TX_HEADER_LEN + MAX_CONTENT_LEN + 3] = "";

  // 送信文字列のヘッダーの作成
  sprintf(txMessage, ":%02xA0%02xFF%02x",
          DOWNSTREAM_TWELITE, NORMAL_ID, THIS_TWELITE);

  // 送信文字列への測定値の追加
  for (int i = 0; i < NUM_OF_MEAS_ITEMS; i++)
    sprintf(txMessage + TX_HEADER_LEN + 2 + i * MEAS_VALUE_LEN,
            "%04x", measValue[i]);

  // 送信文字列へのChecksumの追加
  sprintf(txMessage + TX_HEADER_LEN + 2 + NUM_OF_MEAS_ITEMS * MEAS_VALUE_LEN,
          "%02x", getChecksumOf(txMessage));

  // 送信実行
  toUpperStr(txMessage);
  writeToSerial(txMessage);
}

// 受信メッセージに応じて，チェインオブレスポンシビリティパターンによる適切な処理を行う
// 処理ができた場合にはtrueを，処理ができなかった場合にはfalseを返す
bool parseMessage(MessageHandler *pHandler, const char *rxMessage) {
  RxContent rxContent;

  // 各種データをrxContentに格納する
  rxContent.message = rxMessage;
  sscanf(rxMessage + 1, "%2x", &rxContent.tweliteId);
  sscanf(rxMessage + 5, "%2x", &rxContent.responseId);
  rxContent.contentLength = strlen(rxMessage) - RX_HEADER_LEN - 2;

  // 受信メッセージの内容に応じて，適切に処理する
  return pHandler->handleMessage(pHandler, &rxContent);
}
