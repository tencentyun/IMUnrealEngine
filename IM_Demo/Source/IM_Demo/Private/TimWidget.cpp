// Fill out your copyright notice in the Description page of Project Settings.


#include "TimWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "V2TIMManager.h"
#include "DebugDefs.h"

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

  V2TIMSDKConfig timConfig;
  timConfig.initPath = static_cast<V2TIMString>("D:\\");
  timConfig.logPath = static_cast<V2TIMString>("D:\\");
  // 初始化sdk
  bool isInit = timInstance->InitSDK(SDKAppID, timConfig);
  if(isInit) {
    // 初始化成功
    writeLblLog("===init sucess");
  } else {
    writeLblLog("===init fail");
  }
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