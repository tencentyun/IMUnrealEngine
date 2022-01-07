// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "V2TIMBuffer.h"
#include "V2TIMConversationManager.h"
#include "V2TIMDefine.h"
#include "V2TIMManager.h"
#include "V2TIMMessage.h"
#include "V2TIMMessageManager.h"
#include "V2TIMGroupManager.h"
#include "V2TIMFriendshipManager.h"
#include "V2TIMSignaling.h"
#include "V2TIMSignalingManager.h"
#include "TimWidget.generated.h"

#if PLATFORM_ANDROID
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/Core/Public/Android/AndroidJavaEnv.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#else
#include "GenerateTestUserSig.h"
#endif



/**
 * 
 */
UCLASS()
class UTimWidget : public UUserWidget,public V2TIMAdvancedMsgListener,public V2TIMCallback
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
    UTextBlock* lblVersion;

	V2TIMManager* timInstance;

	void writeLblLog(const char *log);
	void OnRecvNewMessage(const V2TIMMessage &message) override;
	UFUNCTION(BlueprintCallable, Category ="TimDemoFunction")
		void sendMessageToGroup();

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void OnSuccess() override;
    void OnError(int error_code, const V2TIMString &error_message);

	UFUNCTION(BlueprintCallable, Category ="TimDemoFunction")
		void timLogin();
	UFUNCTION(BlueprintCallable, Category ="TimDemoFunction")
		void createGroupNAddListener();
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
		UTextBlock* txtLog;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
		UEditableTextBox* txtInputMessage;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
		UScrollBox* sbMessageList;
};
