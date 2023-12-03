// Demo by LLJ. You just DO WHAT YOU WANT TO.

// ReSharper disable CppMemberFunctionMayBeConst

#include "TankCharacter.h"

#include <Utils.hpp>

#include "SoundManager.h"

ATankCharacter::ATankCharacter()
	: MovementSpeed(500.0f), // 移动速度
	  RotationSpeed(100.0f), // 旋转速度
	  ElevationAngle(30.0f), // 最大仰角
	  DepressionAngle(-20.0f), // 最大俯角
	  ArmorThickness(100.0f), // 装甲厚度
	  ReloadTime(5.0f), // 装填时间
	  MaxAngle(2.0f), // 最大扰动角度
	  MinAngle(0.1f), // 最小扰动角度
	  ShrinkTime(5.0f), // 瞄准时间
	  MaxSpeed(1000.0f), // 最大速度 (暂未使用)
	  Acceleration(10.0f), // 加速度
	  Friction(200.0f), // 摩擦力
	  MaxHealth(100), // 生命值
	  Health(MaxHealth), // 当前生命值
	  LastFireTime(-ReloadTime) // 上次开火时间
{
	// 车身网格
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(RootComponent);

	// 摄像机根节点（用于绕坦克旋转相机）
	CameraRoot = CreateDefaultSubobject<USceneComponent>("CameraRoot");
	CameraRoot->SetupAttachment(BodyMesh);
	// 摄像机组件
	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(CameraRoot);

	// 炮塔网格
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	TurretMesh->SetupAttachment(BodyMesh);

	// 炮管网格
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	BarrelMesh->SetupAttachment(TurretMesh);
	// 开火位置
	FirePosition = CreateDefaultSubobject<USceneComponent>("FirePosition");
	FirePosition->SetupAttachment(BarrelMesh);

	// 履带网格
	TrackMesh = CreateDefaultSubobject<USceneComponent>("TrackMesh");
	TrackMesh->SetupAttachment(BodyMesh);
}

void ATankCharacter::BeginPlay() { Super::BeginPlay(); }

void ATankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyMove(DeltaTime);
}

void ATankCharacter::MoveForward(float AxisValue)
{
	FVector Direction = GetActorForwardVector();
	FVector Force = Direction * AxisValue * Acceleration;

	// 涡轮增压器
	if (TurbochargerComponent != nullptr && TurbochargerComponent->IsLoaded())
		Force *= 1.1f;

	CurrentVelocity += Force;
}

void ATankCharacter::MoveRight(float AxisValue)
{
	if (abs(AxisValue) < 1e-3) return;

	AxisValue *= 0.5f; // 减缓旋转速度
	FRotator Rotation = Controller->GetControlRotation();
	Rotation.Yaw += AxisValue * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	Controller->SetControlRotation(Rotation);

	// 反向旋转相机，保证指向不变
	TurnTurret(-AxisValue);
}

void ATankCharacter::TurnTurret(float AxisValue)
{
	if (TurretComponent == nullptr || !TurretComponent->IsUsable()) return;

	float RotationChange =
		AxisValue * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	CameraRoot->AddLocalRotation(FRotator(0.0f, RotationChange, 0.0f));
	TurretMesh->AddLocalRotation(FRotator(0.0f, RotationChange, 0.0f));

	if (FMath::Abs(AxisValue) > 1e-3) SetMouseMoveTime();
}

void ATankCharacter::CameraUpAndDown(float AxisValue)
{
	float RotationChange =
		AxisValue * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator CameraRotation = Camera->GetRelativeRotation();
	CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch + RotationChange,
	                                    DepressionAngle, ElevationAngle);
	Camera->SetRelativeRotation(CameraRotation);

	if (FMath::Abs(AxisValue) > 1e-3) SetMouseMoveTime();
}

// 根据摩擦力来减缓速度
void ATankCharacter::ApplyMove(float DeltaTime)
{
	if (TrackComponent == nullptr || !TrackComponent->IsUsable()) return;

	FVector VelocityDirection = CurrentVelocity.GetSafeNormal();
	float Speed = CurrentVelocity.Size();

	// move position
	FVector PositionChange = CurrentVelocity * DeltaTime;
	FVector NewPosition = GetActorLocation() + PositionChange;
	SetActorLocation(NewPosition);

	FVector FrictionForce = -VelocityDirection * Friction;
	float Mass = 1;
	FVector FrictionAcceleration = FrictionForce / Mass;

	CurrentVelocity += FrictionAcceleration * DeltaTime;

	if (FMath::Abs(Speed) <=
		FMath::Abs(FrictionAcceleration.Size() * DeltaTime))
	{
		CurrentVelocity = FVector::ZeroVector;
	}
}

// 对炮弹方向添加随机扰动
// Direction: 炮弹的发射方向
// Angle: 扰动的最大角度
FVector AddRandomOffset(const FVector& Direction, float Angle)
{
	// 生成随机扰动
	float RandomAngle = FMath::RandRange(-Angle, Angle);
	FVector RandomOffset =
		FMath::VRandCone(Direction, FMath::DegreesToRadians(RandomAngle));
	return RandomOffset;
}

// 获取鼠标瞄准的进度
float ATankCharacter::GetAimingProgress() const
{
	// 获取鼠标保持不动的时间
	float IdleTime = GetWorld()->GetTimeSeconds() - LastMouseMoveTime;

	// 根据鼠标保持不动的时间进行插值计算
	float Progress = IdleTime / ShrinkTime;
	Progress = FMath::Min(Progress, 1.0f);

	return Progress;
}

float ATankCharacter::GetRemainingReloadTime() const
{
	float ReloadTimeNow = GetWorld()->GetTimeSeconds() - LastFireTime;
	float RemainingReloadTime = ReloadTime - ReloadTimeNow;
	return FMath::Max(RemainingReloadTime, 0.0f);
}

float ATankCharacter::GetAngleOfRandomOffset()
{
	return FMath::Lerp(MaxAngle, MinAngle, GetAimingProgress());
}

void ATankCharacter::SetMouseMoveTime()
{
	LastMouseMoveTime = GetWorld()->GetTimeSeconds();
}

void SetCursorToCenter()
{
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

void ATankCharacter::Fire()
{
	if (BarrelComponent == nullptr || !BarrelComponent->IsUsable()) return;

	SetCursorToCenter();
	FVector LaunchDirection = CalculateLaunchDirection();
	if (LaunchDirection.Length() < 1e-3) return;

	if (GetRemainingReloadTime() > 0.0f) return;

	// 创建炮弹
	if (!ProjectileClass) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (ATankProjectile* Projectile = GetWorld()->SpawnActor<ATankProjectile>(
		ProjectileClass, FirePosition->GetComponentLocation(),
		LaunchDirection.Rotation(), SpawnParams))
	{
		LaunchDirection =
			AddRandomOffset(LaunchDirection, GetAngleOfRandomOffset());
		Projectile->Shooter = this;
		Projectile->Launch(LaunchDirection);
		LastFireTime = GetWorld()->GetTimeSeconds();
		ASoundManager::GetInstance()->PlayFireSound();
	}
}

float ATankCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                 AController* EventInstigator,
                                 AActor* DamageCauser)
{
	Health -= static_cast<int>(DamageAmount);
	if (Health <= 0)
	{
		// TODO: 死亡
		ASoundManager::GetInstance()->PlayDestroyedSound();
	}
	else
		ASoundManager::GetInstance()->PlayHitSound();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

float ATankCharacter::TankTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                     AController* EventInstigator,
                                     AActor* DamageCauser, float InPenetrationAngle,
                                     float InPenetrationDepth, UPrimitiveComponent* HitComponent)
{
	// 获取被击中的网格类型
	ETankMeshType HitMeshType = ETankMeshType::None;
	if (UStaticMeshComponent* StaticMeshComponent =
		Cast<UStaticMeshComponent>(HitComponent))
	{
		HitMeshType = GetTankMeshType(StaticMeshComponent);
	}
	if (HitMeshType == ETankMeshType::None)
	{
		MyLogError("Not a valid tank mesh type");
		return 0;
	}

	// 计算等效装甲
	float EffectiveArmor = ArmorThickness / FMath::Cos(FMath::DegreesToRadians(InPenetrationAngle));

	if (EffectiveArmor > InPenetrationDepth)
	{
		ASoundManager::GetInstance()->PlayRicochetSound();
		return 0;
	}

	switch (HitMeshType)
	{
	case ETankMeshType::Barrel: // 击中炮管，无伤害，炮管会被击毁
		BarrelComponent->Destroy();
		return 0;
	case ETankMeshType::Body: // 击中车体，造成伤害
		DamageAmount *= FMath::FRandRange(0.95f, 1.05f);
		return TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	case ETankMeshType::Turret: // 击中炮塔，概率击毁炮塔或造成伤害
		if (FMath::RandBool()) // 击毁炮塔
			TurretComponent->Destroy();
		else // 造成伤害
		{
			DamageAmount *= FMath::FRandRange(0.95f, 1.05f);
			return TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		}
		return 0;
	case ETankMeshType::Track: // 击中履带，无伤害，履带会被击毁
		TrackComponent->Destroy();
		return 0;
	default: MyLogError("Add new mesh type to switch statement at ATankCharacter::TankTakeDamage");
		return 0;
	}
}

// 计算炮弹的发射方向
FVector ATankCharacter::CalculateLaunchDirection() const
{
	// 获取鼠标位置下的碰撞点
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(
		ECC_Visibility, false, HitResult);

	// 计算炮弹的发射方向
	FVector LaunchDirection = FVector::ZeroVector;
	if (HitResult.bBlockingHit)
	{
		LaunchDirection =
			HitResult.ImpactPoint - FirePosition->GetComponentLocation();
		LaunchDirection.Normalize();
	}

	return LaunchDirection;
}

void ATankCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 旋转相机
	PlayerInputComponent->BindAxis("Turn", this, &ATankCharacter::TurnTurret);
	PlayerInputComponent->BindAxis("LookUp", this,
	                               &ATankCharacter::CameraUpAndDown);

	// 移动
	PlayerInputComponent->BindAxis("MoveForward", this,
	                               &ATankCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATankCharacter::MoveRight);

	// 开火事件
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this,
	                                 &ATankCharacter::Fire);
}

ETankMeshType ATankCharacter::GetTankMeshType(const UStaticMeshComponent* TankMesh) const
{
	// 注意：不要更改 if 的顺序
	// 履带
	if (TankMesh->GetAttachParent() == TrackMesh) return ETankMeshType::Track;
	// 炮管
	if (TankMesh == BarrelMesh || TankMesh->GetAttachParent() == BarrelMesh)
		return ETankMeshType::Barrel;
	// 炮塔
	if (TankMesh == TurretMesh || TankMesh->GetAttachParent() == TurretMesh)
		return ETankMeshType::Turret;
	// 车身
	if (TankMesh == BodyMesh || TankMesh->GetAttachParent() == BodyMesh)
		return ETankMeshType::Body;
	return ETankMeshType::None;
}
