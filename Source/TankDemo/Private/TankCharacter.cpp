// Demo by LLJ. You just DO WHAT YOU WANT TO.


#include "TankCharacter.h"

// Sets default values
ATankCharacter::ATankCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATankCharacter::MoveForward(float AxisValue)
{
	FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, AxisValue * MovementSpeed);
}

void ATankCharacter::MoveRight(float AxisValue)
{
	FVector Right = GetActorRightVector();
	AddMovementInput(Right, AxisValue * MovementSpeed);
}

void ATankCharacter::RotateTurret(float AxisValue)
{
	float RotationChange = AxisValue * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	TurretRotation.Yaw += RotationChange;
}

void ATankCharacter::AdjustBarrel(float AxisValue)
{
	float RotationChange = AxisValue * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	BarrelRotation.Pitch += RotationChange;
	BarrelRotation.Pitch = FMath::Clamp(BarrelRotation.Pitch, -45.0f, 45.0f);
}

void ATankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 更新炮塔和炮管的旋转
	TurretMesh->SetRelativeRotation(TurretRotation);
	BarrelMesh->SetRelativeRotation(BarrelRotation);
}

void ATankCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 绑定鼠标旋转事件
	PlayerInputComponent->BindAxis("Turn", this, &ATankCharacter::RotateTurret);
	PlayerInputComponent->BindAxis("LookUp", this, &ATankCharacter::AdjustBarrel);

	// 绑定WASD键事件
	PlayerInputComponent->BindAxis("MoveForward", this, &ATankCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATankCharacter::MoveRight);
}