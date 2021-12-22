// Copyright Epic Games, Inc. All Rights Reserved.

#include "IM_DemoGameMode.h"
#include "IM_DemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AIM_DemoGameMode::AIM_DemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
