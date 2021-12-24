// Fill out your copyright notice in the Description page of Project Settings.


#include "TimWidget.h"
#include "Engine/Engine.h"
#include <string>
// #include "TIMCloud.h"
#include "V2TIMManager.h"
#include "DebugDefs.h"

void UTimWidget::NativeConstruct()
{
  Super::NativeConstruct();
  

  V2TIMManager* timInstance = V2TIMManager::GetInstance();
  V2TIMString timString = timInstance->GetVersion();

  // std::string stdStrTemp2(TIMGetSDKVersion());
  std::string stdStrTemp2(timString.CString());
  FString tempUserText = stdStrTemp2.c_str();
  lblVersion->SetText(FText::FromString(tempUserText));
}

void UTimWidget::NativeDestruct()
{
  Super::NativeDestruct();
}