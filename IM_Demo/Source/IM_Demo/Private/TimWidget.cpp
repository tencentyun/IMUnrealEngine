// Fill out your copyright notice in the Description page of Project Settings.


#include "TimWidget.h"
#include "Engine/Engine.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "HAL/Platform.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include <string>
#include <stdlib.h>
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
  #if PLATFORM_ANDROID
    timConfig.initPath = static_cast<V2TIMString>("D:\\");
    timConfig.logPath = static_cast<V2TIMString>("D:\\");
  #endif
  // #if PLATFORM_MAC || PLATFORM_IOS
  // 	FString rootDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	//   FString initDir = rootDir + TEXT("Documents/com_tencent_imsdk_data/");
  //   timConfig.initPath = static_cast<V2TIMString>(TCHAR_TO_ANSI(*initDir));
  //   FString logDir = rootDir + TEXT("Library/Caches/com_tencent_imsdk_log/");
  //   timConfig.logPath = static_cast<V2TIMString>(TCHAR_TO_ANSI(*logDir));
  // #endif
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

void UTimWidget::timLogin() {
  class LoginCallback : public V2TIMCallback {
    private:
      UTimWidget* _TimWidget;
    public:
      LoginCallback(UTimWidget* widget) {
        this->_TimWidget = widget;
      };
      ~LoginCallback() {

      };
      void OnSuccess() override {
        UE_LOG(LogTemp,Log,TEXT("<== login OnSuccess"));
        this->_TimWidget->createGroupNAddListener();
      };
      void OnError(int error_code, const V2TIMString &error_message) override {
        this->_TimWidget->writeLblLog("login failed");
        UE_LOG(LogTemp,Log,TEXT("<== login failed OnError ======: %d"), error_code);
      };
  };
  LoginCallback* login_callback_ = new LoginCallback(this);
  FString tempUid = FGuid::NewGuid().ToString();
  FString guidUser = tempUid.Left(7);
  // https://answers.unrealengine.com/questions/49838/fguid-doesnt-create-unique-guids-on-os-x.html
  #if PLATFORM_MAC || PLATFORM_IOS
  int r = rand() + rand();
  std::string sRand = std::to_string(r);
  const char* imTestUserId = sRand.c_str();
  #else
  const char* imTestUserId = TCHAR_TO_ANSI(*guidUser);
  #endif
  #if PLATFORM_ANDROID
  if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
      jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "genTestUserSig", "(ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
      jstring jsUserId = Env->NewStringUTF(imTestUserId);
      jstring jsKey = Env->NewStringUTF(SECRETKEY);
      jstring JstringResult = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis,GetPackageNameMethodID, SDKAppID, jsUserId, jsKey);
      FString FinalResult = FJavaHelper::FStringFromLocalRef(Env, JstringResult);
      auto twoHundredAnsi = StringCast<ANSICHAR>(*FinalResult);
      const char* userSig = twoHundredAnsi.Get();
      timInstance->Login(static_cast<V2TIMString>(imTestUserId), static_cast<V2TIMString>(userSig), login_callback_);
  }
  #else
    const char* userSig = GenerateTestUserSig().genTestUserSig(imTestUserId, SDKAppID, SECRETKEY);
    timInstance->Login(static_cast<V2TIMString>(imTestUserId), static_cast<V2TIMString>(userSig), login_callback_);
  #endif
}


V2TIMString groupID = "test-UE4-IM";
class SendCallback : public V2TIMSendCallback {
  public:
    ~SendCallback() override {};
    void OnSuccess(const V2TIMMessage &message) override{
        UE_LOG(LogTemp,Log,TEXT("=== SendCallback OnSuccess ======"));
    };
    void OnError(int error_code, const V2TIMString &error_message) override {
        UE_LOG(LogTemp,Log,TEXT("=== SendCallback OnError error code:%d ======"),error_code);
    };
    void OnProgress(uint32_t progress) override {
      UE_LOG(LogTemp,Log,TEXT("=== SendCallback OnProgress progress:%d ======"),progress);
    };
};
SendCallback send_callback_;
void UTimWidget::sendMessageToGroup() {
  FString fInputMessage = txtInputMessage->GetText().ToString();
  std::string strInputMessage(TCHAR_TO_UTF8(*fInputMessage));
  const char* charInputMessage = strInputMessage.c_str();
  if(strlen(charInputMessage) == 0)
    return;
  auto message = V2TIMManager::GetInstance()->GetMessageManager()->CreateTextMessage(
            charInputMessage);
  this->writeLblLog("===CreateTextMessage====");          
  V2TIMManager::GetInstance()->GetMessageManager()->SendMessage(
      message, "", groupID, V2TIM_PRIORITY_DEFAULT, false,
      V2TIMOfflinePushInfo(),&send_callback_);

  UTextBlock* textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
  FString me = FString(TEXT("me: "));
  FString msg  = txtInputMessage->GetText().ToString();
  textBlock->SetText(FText::FromString(me + msg));
  textBlock->SetAutoWrapText(true);
  sbMessageList->AddChild(
    textBlock
  );
  sbMessageList->ScrollToEnd();
  txtInputMessage->SetText(FText::FromString(""));
}

void UTimWidget::OnSuccess() {
  UE_LOG(LogTemp,Log,TEXT("<==== JoinGroup OnSuccess ======"));
}
void UTimWidget::OnError(int error_code, const V2TIMString &error_message){
  UE_LOG(LogTemp,Log,TEXT("<===== JoinGroup OnError  errorCode: %d") ,error_code);
}

void UTimWidget::OnRecvNewMessage(const V2TIMMessage &v2_message) {
  UE_LOG(LogTemp,Log,TEXT("==>OnRecvNewMessage ======"));
  V2TIMSignalingInfo info = V2TIMManager::GetInstance()->GetSignalingManager()->GetSignalingInfo(v2_message);

  for (unsigned long j = 0; j < v2_message.elemList.Size(); ++j) {
      switch (v2_message.elemList[j]->elemType) {
          case V2TIM_ELEM_TYPE_TEXT:{
            AsyncTask(ENamedThreads::GameThread, [=]()
            {
              const char * msgEle = ((V2TIMTextElem *)v2_message.elemList[j])->text.CString();
              UTextBlock* textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
              FString other =  v2_message.sender.CString();
              FString ele = UTF8_TO_TCHAR(msgEle);// FString(TEXT("&msgEle"));
              textBlock->SetText(FText::FromString(other + FString(TEXT(": ")) + ele));
              textBlock->SetAutoWrapText(true);
              sbMessageList->AddChild(
                textBlock
              );
              sbMessageList->ScrollToEnd();
           });
          }break;
          case V2TIM_ELEM_TYPE_CUSTOM:
              break;
          case V2TIM_ELEM_TYPE_IMAGE:
              break;
          case V2TIM_ELEM_TYPE_SOUND:
              break;
          case V2TIM_ELEM_TYPE_VIDEO:
              break;
          case V2TIM_ELEM_TYPE_FILE:
              break;
          case V2TIM_ELEM_TYPE_LOCATION:
              break;
          case V2TIM_ELEM_TYPE_FACE:
              break;
          case V2TIM_ELEM_TYPE_GROUP_TIPS:
              break;
          case V2TIM_ELEM_TYPE_MERGER:
              break;
          default:
              break;
      }
  }
}

void UTimWidget::createGroupNAddListener() {
  V2TIMGroupInfo group_info;
  group_info.groupID = groupID;
  group_info.groupType = "Public";
  group_info.groupName = "这是一个直播群组";
  group_info.groupAddOpt = V2TIM_GROUP_ADD_ANY;
  group_info.introduction = "群简介";

  V2TIMCreateGroupMemberInfo member;
  member.userID = testUserId;
  member.role = V2TIM_GROUP_MEMBER_ROLE_MEMBER;
  V2TIMCreateGroupMemberInfoVector member_list;
  V2TIMManager::GetInstance()->GetGroupManager()->CreateGroup(
      group_info, member_list, nullptr);

  V2TIMManager::GetInstance()->GetMessageManager()->AddAdvancedMsgListener(this);
  // JoinGroup
  V2TIMString message = "join group hello-UE4-IM";
  V2TIMManager::GetInstance()->JoinGroup(groupID, message, this);
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
