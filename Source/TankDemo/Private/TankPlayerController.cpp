// Demo by LLJ. You just DO WHAT YOU WANT TO.

#include "TankPlayerController.h"
#include "Utils.hpp"

ATankPlayerController::ATankPlayerController()
{
	bEnableMouseOverEvents = true;
	bEnableClickEvents = false;
	bShowMouseCursor = false;
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UGameViewportClient* GameViewportClient = GetWorld()->GetGameViewport())
	{
		GameViewportClient->SetMouseLockMode(EMouseLockMode::LockOnCapture);
	}
}
