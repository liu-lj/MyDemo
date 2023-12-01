// Demo by LLJ. You just DO WHAT YOU WANT TO.


#include "TankProjectile.h"

#include <Utils.hpp>

#include "Kismet/GameplayStatics.h"

ATankProjectile::ATankProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 创建碰撞组件
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionProfileName("Projectile");

	// 创建移动组件
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 2000.0f;
	ProjectileMovementComponent->MaxSpeed = 2000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// 设置碰撞回调函数
	CollisionComponent->OnComponentHit.AddDynamic(this, &ATankProjectile::OnProjectileHit);
}

void ATankProjectile::Launch(const FVector& Direction) const
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

void ATankProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 在碰撞点播放粒子效果
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint);

		// print hit info
		// 1. print hit actor name
		MyLog(OtherActor->GetName());
		// 2. print hit component name
		MyLog(OtherComp->GetName());
		// 3. print normal impulse
		MyLog(FString::Printf(TEXT("NormalImpulse: %s"), *NormalImpulse.ToString())); 
		// 4. print direction
		MyLog(FString::Printf(TEXT("Direction: %s"), *Hit.ImpactNormal.ToString()));
		// 5. print dot(normal, direction) (assumed to be >= 0)
		MyLog(FString::SanitizeFloat(FVector::DotProduct(NormalImpulse, Hit.ImpactNormal)));
	}

	// 销毁炮弹
	Destroy();
}

void ATankProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ATankProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
