// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TankCharacter.generated.h"

UCLASS()
class ATankCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATankCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* BarrelMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	FRotator TurretRotation;
	UPROPERTY(EditAnywhere, Category = "Components")
	FRotator BarrelRotation;

	UPROPERTY(EditAnywhere, Category = "Components")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Components")
	float RotationSpeed;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void RotateTurret(float AxisValue);
	void AdjustBarrel(float AxisValue);
};