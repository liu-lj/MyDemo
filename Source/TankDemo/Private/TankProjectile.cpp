// Demo by LLJ. You just DO WHAT YOU WANT TO.

#include "TankProjectile.h"

#include <Utils.hpp>

#include "TankCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

ATankProjectile::ATankProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 创建碰撞组件
	CollisionComponent =
		CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionProfileName("Projectile");

	// 创建移动组件
	ProjectileMovementComponent =
		CreateDefaultSubobject<UProjectileMovementComponent>(
			TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 2000.0f;
	ProjectileMovementComponent->MaxSpeed = 2000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// 设置碰撞回调函数
	CollisionComponent->OnComponentHit.AddDynamic(
		this, &ATankProjectile::OnProjectileHit);
}

void ATankProjectile::Launch(const FVector& Direction) const
{
	ProjectileMovementComponent->Velocity =
		Direction * ProjectileMovementComponent->InitialSpeed;
}

void ATankProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent,
                                      AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      FVector NormalImpulse,
                                      const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 在碰撞点播放粒子效果
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect,
		                                         Hit.ImpactPoint);

		// try convert OtherActor to ATankCharacter
		if (ATankCharacter* TankCharacter = Cast<ATankCharacter>(OtherActor))
		{
			// 伤害
			TankCharacter->TakeDamage(100.0f, FDamageEvent(),
			                          GetInstigatorController(), this);
			
			if (UStaticMeshComponent* StaticMeshComponent =
				Cast<UStaticMeshComponent>(OtherComp))
			{
				switch (TankCharacter->GetTankMeshType(StaticMeshComponent))
				{
				case ETankMeshType::Body:
					break;
				case ETankMeshType::Turret:
					break;
				case ETankMeshType::Barrel:
					break;
				case ETankMeshType::Track:
					break;
				case ETankMeshType::None:
					MyLogError("Not a valid tank mesh type");
					break;
				}
			}
			// else convert to collision cylinder
			else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(OtherComp))
			{
			}

			FVector ImpactNormal = Hit.ImpactNormal;
			float Angle = FMath::Acos(
				FVector::DotProduct(ImpactNormal, -ProjectileMovementComponent->Velocity.GetSafeNormal()));
			float Degrees = FMath::RadiansToDegrees(Angle);
			MyLog(FString::Printf(TEXT("Angle = %f"), Degrees));
		}
	}

	// 销毁炮弹
	Destroy();
}

void ATankProjectile::BeginPlay() { Super::BeginPlay(); }

void ATankProjectile::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
