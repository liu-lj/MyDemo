// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "TankCharacter.generated.h"

UCLASS()
class ATankCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATankCharacter();
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	void ApplyFriction(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere, Category = "Tank")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, Category = "Tank")
	USceneComponent* CameraRoot;
	UPROPERTY(EditAnywhere, Category = "Tank")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Tank")
	UStaticMeshComponent* TurretMesh;
	
	UPROPERTY(EditAnywhere, Category = "Tank")
	UStaticMeshComponent* BarrelMesh;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Tank")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Acceleration;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Friction;

	FVector CurrentVelocity;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void TurnTurret(float AxisValue);
	void CameraUpAndDown(float AxisValue);
};