// Copyright Epic Games, Inc. All Rights Reserved.

#include "TankDemoGameMode.h"
#include "TankDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATankDemoGameMode::ATankDemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
