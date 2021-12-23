// Fill out your copyright notice in the Description page of Project Settings.


#include "TimWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "TIMCloud.h"
#include "DebugDefs.h"

void UTimWidget::NativeConstruct()
{
  Super::NativeConstruct();
  std::string stdStrTemp2(TIMGetSDKVersion());
  FString tempUserText = stdStrTemp2.c_str();
  lblVersion->SetText(FText::FromString(tempUserText));
}

void UTimWidget::NativeDestruct()
{
  Super::NativeDestruct();
}