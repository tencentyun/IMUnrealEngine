// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
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
class UTimWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
    UTextBlock* lblVersion;
	void writeLblLog(const char *log);
	void timLogin();
	void joinGroup();
	void sendMessageToGroup();

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
			UTextBlock* txtLog;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
			UScrollBox* sbMessageList;
};
