//        10        20        30        40        50        60        70        80        90        100
#ifndef HANDLER_H
#define HANDLER_H

// 引数にMessageHandler型のポインタとRxContent型のポインタをもち，
// bool型の変数を返す関数へのポインタを保持する構造体
typedef struct _MessageHandler {
  bool (* const handleMessage)(struct _MessageHandler *pThis, RxContent *pContent);
} MessageHandler;

// MessageHandlerとそれ以外に必要なデータを格納する構造体
// 擬似的にMessageHandlerを継承した構造体となっている
typedef struct {
  MessageHandler base;
  const unsigned int responseId;
  const int contentLength;
} RxMsgHandler;

// 処理の連鎖を格納するための構造体
// 擬似的にMessageHandlerを継承した構造体となっている
typedef struct {
  MessageHandler base;
  MessageHandler *pWrapped;
  MessageHandler *pNext;
} ChainedHandler;

// 受信メッセージの処理を試みて，処理ができた場合にはtrueを返す
// 処理がができなかった場合には、後続のHandlerを呼び出し，
// 後続のHandlerの戻り値をそのまま呼び出し元に返す
// 後続のHandlerがいない場合にはfalseを返す
bool isHandled(MessageHandler *pThis, RxContent *pContent);

// 不要な受信文字列を無視するHandler
bool handleIgnore(MessageHandler *pThis, RxContent *pContent);

// 通常の受信メッセージを下流側のデバイスに送信するHandler
bool handleNormalMsg(MessageHandler *pThis, RxContent *pContent);
#endif
