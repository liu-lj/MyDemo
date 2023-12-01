// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/GameViewportClient.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	ATankPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
