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

	MyLog("SET MOUSE LOCK MODE = LockOnCapture");
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 获取当前视图大小
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// 设置鼠标位置为屏幕中心
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
	{
		FViewport* Viewport = GEngine->GameViewport->Viewport;
		Viewport->SetMouse(ViewportSize.X / 2, ViewportSize.Y / 2);
	}
}