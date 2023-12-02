// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "DamageableTank.h"
#include "TankProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "TankCharacter.generated.h"

UENUM()
enum class ETankMeshType : uint8
{
	Body UMETA(DisplayName = "Body"),
	Turret UMETA(DisplayName = "Turret"),
	Barrel UMETA(DisplayName = "Barrel"),
	Track UMETA(DisplayName = "Track"),
	None UMETA(DisplayName = "None")
};

UCLASS()
class ATankCharacter : public ACharacter  , public IDamageableTank
{
	GENERATED_BODY()

public:
	ATankCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;
	ETankMeshType GetTankMeshType(const UStaticMeshComponent* TankMesh) const;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetAimingProgress() const;
	// 装填时间
	UFUNCTION(BlueprintCallable)
	float GetRemainingReloadTime() const;

	virtual float TankTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                             AActor* DamageCauser, float InPenetrationAngle, float InPenetrationDepth,
	                             UPrimitiveComponent* HitComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* CameraRoot;
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BarrelMesh;
	UPROPERTY(EditAnywhere)
	USceneComponent* FirePosition;

	UPROPERTY(EditAnywhere)
	USceneComponent* TrackMesh;

	UPROPERTY(EditAnywhere)
	UAudioComponent* Audio;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATankProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Tank")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float ElevationAngle; // 仰角
	UPROPERTY(EditAnywhere, Category = "Tank")
	float DepressionAngle; // 俯角

	UPROPERTY(EditAnywhere, Category = "Tank")
	float ArmorThickness;

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

	FVector CurrentVelocity;
	float LastMouseMoveTime;
	float LastFireTime;
};
