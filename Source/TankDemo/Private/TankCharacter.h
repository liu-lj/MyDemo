// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "TankProjectile.h"
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

	UFUNCTION(BlueprintCallable, Category = "Tank")
	float GetAimingProgress() const;
	// 装填时间
	UFUNCTION(BlueprintCallable, Category = "Tank")
	float GetRemainingReloadTime() const;

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
	USceneComponent* FirePosition;

	UPROPERTY(EditAnywhere, Category = "Tank")
	TSubclassOf<ATankProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Tank")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float ReloadTime;
	UPROPERTY(EditAnywhere, Category = "Tank")
	float MaxAngle;
	UPROPERTY(EditAnywhere, Category = "Tank")
	float MinAngle;
	UPROPERTY(EditAnywhere, Category = "Tank")
	float ShrinkTime;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float Acceleration;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float Friction;

	FVector CurrentVelocity;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void TurnTurret(float AxisValue);
	void CameraUpAndDown(float AxisValue);
	void ApplyFriction(float DeltaTime);
	float GetAngleOfRandomOffset();
	void SetMouseMoveTime();
	void Fire();
	FVector CalculateLaunchDirection() const;

	float LastMouseMoveTime;
	float LastFireTime;
};
