// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TankProjectile.generated.h"

class ATankCharacter;

UCLASS()
class ATankProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATankProjectile();
	void Launch(const FVector& Direction) const;
	void MakeExplosionEffect(const FVector& Location,
	                         const FRotator& Rotation) const;

	ATankCharacter* Shooter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "TankProjectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "TankProjectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = "TankProjectile")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "TankProjectile")
	float PenetrationDepth;

	UPROPERTY(EditDefaultsOnly, Category = "TankProjectile")
	float MaxDamage;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                     const FHitResult& Hit);

public:
	virtual void Tick(float DeltaTime) override;
};
