#ifndef UTIL_H
#define UTIL_H
#include "peripheral.h"

// チェックサムを計算するための関数
int getChecksumOf(const char *);

// 大文字に変換するための関数
void toUpperStr(const char *);

// 標準のシリアルポートとSoftwareSerialで擬似的に作成された
// シリアルポートの両方に書き込みを行うための関数
void writeToSerial(const char *);

#endif
