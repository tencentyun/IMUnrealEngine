// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "TimWidget.generated.h"

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

public:
	void NativeConstruct() override;
  void NativeDestruct() override;
	
};
