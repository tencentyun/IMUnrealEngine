// Fill out your copyright notice in the Description page of Project Settings.


#include "TimWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "DebugDefs.h"



void UTimWidget::NativeConstruct()
{
  Super::NativeConstruct();
  
  // 获取sdk单例对象
  timInstance = V2TIMManager::GetInstance();
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
    writeLblLog("===init sucess====");
    this->timLogin();
  } else {
    writeLblLog("===init fail====");
  }
  sbMessageList->ClearChildren();
}

void UTimWidget::OnSuccess() {
  writeLblLog("OnSuccess");
  this->joinGroup();
}

void UTimWidget::OnError (int error_code, const V2TIMString& error_message) {
  writeLblLog(error_message.CString());
}

void UTimWidget::timLogin() {
  #if PLATFORM_ANDROID
  if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
      jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "genTestUserSig", "(ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
      jstring jsUserId = Env->NewStringUTF(testUserId);
      jstring jsKey = Env->NewStringUTF(SECRETKEY);
      jstring JstringResult = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis,GetPackageNameMethodID, SDKAppID, jsUserId, jsKey);
      FString FinalResult = FJavaHelper::FStringFromLocalRef(Env, JstringResult);
      auto twoHundredAnsi = StringCast<ANSICHAR>(*FinalResult);
      const char* userSig = twoHundredAnsi.Get();
      timInstance->Login(static_cast<V2TIMString>(testUserId), static_cast<V2TIMString>(userSig), this);
  }
  #else
    const char* userSig = GenerateTestUserSig().genTestUserSig(testUserId, SDKAppID, SECRETKEY);
    timInstance->Login(static_cast<V2TIMString>(testUserId), static_cast<V2TIMString>(userSig), this);
  #endif
}

void UTimWidget::addMessageToUI(const char *message){
  UTextBlock* textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
  textBlock->SetText(FText::FromString(message));
  sbMessageList->AddChild(
    textBlock
  );
}


void UTimWidget::sendMessageToGroup() {
  FString fInputMessage = txtInputMessage->GetText().ToString();
  std::string strInputMessage(TCHAR_TO_UTF8(*fInputMessage));
  const char* charInputMessage = strInputMessage.c_str();

  auto message = V2TIMManager::GetInstance()->GetMessageManager()->CreateTextMessage(
            charInputMessage);
  V2TIMManager::GetInstance()->GetMessageManager()->SendMessage(
      message, "", "hello-UE4-IM", V2TIM_PRIORITY_DEFAULT, false,
      V2TIMOfflinePushInfo(),nullptr);
  UTextBlock* textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
  textBlock->SetText(txtInputMessage->GetText());
  sbMessageList->AddChild(
    textBlock
  );
  txtInputMessage->SetText(FText::FromString(""));
}

void UTimWidget::joinGroup() {
  V2TIMString groupID = "hello-UE4-IM";
  V2TIMGroupInfo group_info;
  group_info.groupID = groupID;
  group_info.groupType = "Public";
  group_info.groupName = "这是一个直播群组";
  group_info.groupAddOpt = V2TIM_GROUP_ADD_AUTH;
  group_info.introduction = "群简介";

  V2TIMCreateGroupMemberInfo member;
  member.userID = testUserId;
  member.role = V2TIM_GROUP_MEMBER_ROLE_MEMBER;
  V2TIMCreateGroupMemberInfoVector member_list;
  //member_list.PushBack(member);
  V2TIMManager::GetInstance()->GetGroupManager()->CreateGroup(
      group_info, member_list, nullptr);

  V2TIMString message = "join group hello-UE4-IM";
  V2TIMManager::GetInstance()->JoinGroup(groupID, message, nullptr);
  addMessageToUI("I join group hello-UE4-IM");

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
