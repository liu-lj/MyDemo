// Copyright Epic Games, Inc. All Rights Reserved.

#include "Homework1GameMode.h"
#include "Homework1Character.h"
#include "UObject/ConstructorHelpers.h"

AHomework1GameMode::AHomework1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
