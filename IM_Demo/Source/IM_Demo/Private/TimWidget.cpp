// Fill out your copyright notice in the Description page of Project Settings.


#include "TimWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "DebugDefs.h"

#include "V2TIMBuffer.h"
#include "V2TIMDefine.h"
#include "V2TIMManager.h"
#include "V2TIMMessage.h"
#include "V2TIMGroupManager.h"


class LoginV2TIMCallback : public V2TIMCallback {
  public:
    LoginV2TIMCallback() {}
    void OnSuccess() {
    }
    void OnError(int error_code, const V2TIMString& error_message) {
    }
};

void UTimWidget::NativeConstruct()
{
  Super::NativeConstruct();
  
  // 获取sdk单例对象
  V2TIMManager* timInstance = V2TIMManager::GetInstance();
  // 获取sdk版本号
  V2TIMString timString = timInstance->GetVersion();

  std::string stdStrTemp2(timString.CString());
  FString tempUserText = stdStrTemp2.c_str();
  lblVersion->SetText(FText::FromString(tempUserText));

  V2TIMString ui_platform = "ue4";
  timInstance->CallExperimentalAPI("setUIPlatform", &ui_platform, nullptr);
  
  V2TIMSDKConfig timConfig;
  timConfig.initPath = static_cast<V2TIMString>("D:\\");
  timConfig.logPath = static_cast<V2TIMString>("D:\\");
  // 初始化sdk
  bool isInit = timInstance->InitSDK(SDKAppID, timConfig);
  if(isInit) {
    // 初始化成功
    writeLblLog("===init sucess");
    this->timLogin();
  } else {
    writeLblLog("===init fail");
  }
  sbMessageList->ClearChildren();
  
}

void UTimWidget::timLogin() {
  V2TIMCallback* timCallBack = new LoginV2TIMCallback();
  #if PLATFORM_ANDROID
  if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
      jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "genTestUserSig", "(ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
      jstring jsUserId = Env->NewStringUTF(testUserId);
      jstring jsKey = Env->NewStringUTF(SECRETKEY);
      jstring JstringResult = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis,GetPackageNameMethodID, SDKAppID, jsUserId, jsKey);
      FString FinalResult = FJavaHelper::FStringFromLocalRef(Env, JstringResult);
      auto twoHundredAnsi = StringCast<ANSICHAR>(*FinalResult);
      const char* userSig = twoHundredAnsi.Get();
      timInstance->Login(static_cast<V2TIMString>(testUserId), static_cast<V2TIMString>(userSig), timCallBack);
  }
  #else
    const char* userSig = GenerateTestUserSig().genTestUserSig(testUserId, SDKAppID, SECRETKEY);
    timInstance->Login(static_cast<V2TIMString>(testUserId), static_cast<V2TIMString>(userSig), timCallBack);
  #endif
}

void UTimWidget::sendMessageToGroup() {
  UTextBlock* textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
  TextBlock->SetText(FText::FromString("我：发送消息"));
  sbMessageList->AddChild(
    textBlock
  );
}

void UTimWidget::joinGroup() {
  UTextBlock* textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
  TextBlock->SetText(FText::FromString("我进入群聊"));
  sbMessageList->AddChild(
    textBlock
  );
}

void UTimWidget::NativeDestruct()
{
  Super::NativeDestruct();
}

void UTimWidget::writeLblLog(const char * logStr) {
    std::string stdStrLog(logStr);
    FString log = stdStrLog.c_str();
    UE_LOG(LogTemp,Log,TEXT("==> %s"), *log);
    if (txtLog != nullptr) {
        AsyncTask(ENamedThreads::GameThread, [=]() {
            txtLog->SetText(FText::FromString(log));
        });
        
    }else{
        AsyncTask(ENamedThreads::GameThread, [=]()
        {
            UE_LOG(LogTemp,Warning, TEXT("txtLog not find"));
        });
    }
}
