#include "handler.h"
#include "peripheral.h"

bool isHandled(MessageHandler *pHandler, RxContent *pContent) {
  ChainedHandler *pThis = (ChainedHandler *)pHandler;

  pHandler = pThis->pWrapped;
  if (pHandler->handleMessage(pHandler, pContent))
    return true;

  pHandler = pThis->pNext;
  if (pHandler == NULL)
    return false;
  
  return pHandler->handleMessage(pHandler, pContent);
}

bool handleIgnore(MessageHandler *pHandler, RxContent *pContent) {
  // 下記の条件の場合には即退出
  if (pContent->tweliteId != IGNORE_TWELITE
      && pContent->contentLength <= MAX_CONTENT_LEN)
      return false;

  // 何もせず終了
  Serial.println("Handler: Ignore");
  return true;
}

bool handleNormalMsg(MessageHandler *pHandler, RxContent *pContent) {
  RxMsgHandler *pThis = (RxMsgHandler *)pHandler;

  // 下記の条件の場合には即退出
  if (pContent->responseId != pThis->responseId)
      return false;
  if ((pContent->contentLength % MEAS_VALUE_LEN) != 2)
      return false;

  char txMessage[TX_HEADER_LEN + MAX_CONTENT_LEN + 3];
  sprintf(txMessage, ":%02xA0%02xFF", DOWNSTREAM_TWELITE, pThis->responseId);
  for (int i = 0; i < pContent->contentLength; i++)
    txMessage[i + TX_HEADER_LEN] = rxMessage[i + RX_HEADER_LEN];
  txMessage[TX_HEADER_LEN + pContent->contentLength] = '\0';
  sprintf(txMessage + (TX_HEADER_LEN + pContent->contentLength),
          "%02x", getChecksumOf(txMessage));
          
  toUpperStr(txMessage);
  writeToSerial(txMessage);
  
  Serial.println("Handler: NormalMessage");
  return true;
}
