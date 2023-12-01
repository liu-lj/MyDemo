// Demo by LLJ. You just DO WHAT YOU WANT TO.

// ReSharper disable CppMemberFunctionMayBeConst

#include "TankCharacter.h"

ATankCharacter::ATankCharacter() :
	MovementSpeed(500.0f),
	RotationSpeed(100.0f),
	MaxSpeed(1000.0f),
	Acceleration(10.0f),
	Friction(200.0f)
{
	// 车身网格
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);

	// 摄像机根节点（用于绕坦克旋转相机）
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(BodyMesh);
	// 摄像机组件
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(CameraRoot);

	// 炮塔网格
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	// 炮管网格
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BarrelMesh->SetupAttachment(TurretMesh);
	// 开火位置
	FirePosition = CreateDefaultSubobject<USceneComponent>(TEXT("FirePosition"));
	FirePosition->SetupAttachment(BarrelMesh);
}

void ATankCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyFriction(DeltaTime);
}

void ATankCharacter::MoveForward(float AxisValue)
{
	FVector Direction = GetActorForwardVector();
	FVector Force = Direction * AxisValue * Acceleration;
	CurrentVelocity += Force;
}

void ATankCharacter::MoveRight(float AxisValue)
{
	FVector Direction = GetActorRightVector();
	FVector Force = Direction * AxisValue * Acceleration;
	CurrentVelocity += Force;
}

void ATankCharacter::TurnTurret(float AxisValue)
{
	float RotationChange = AxisValue * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	CameraRoot->AddLocalRotation(FRotator(0.0f, RotationChange, 0.0f));
	TurretMesh->AddLocalRotation(FRotator(0.0f, RotationChange, 0.0f));

	SetMouseMoveTime();
}

void ATankCharacter::CameraUpAndDown(float AxisValue)
{
	float RotationChange = AxisValue * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator CameraRotation = Camera->GetRelativeRotation();
	CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch + RotationChange, -50.0f, 20.0f);
	Camera->SetRelativeRotation(CameraRotation);

	SetMouseMoveTime();
}

// 根据摩擦力来减缓速度
void ATankCharacter::ApplyFriction(float DeltaTime)
{
	FVector VelocityDirection = CurrentVelocity.GetSafeNormal();
	float Speed = CurrentVelocity.Size();

	// move position
	FVector PositionChange = CurrentVelocity * DeltaTime;
	FVector NewPosition = GetActorLocation() + PositionChange;
	SetActorLocation(NewPosition);

	FVector FrictionForce = -VelocityDirection * Friction;
	// float mass = GetMass();
	float Mass = 1;
	FVector FrictionAcceleration = FrictionForce / Mass;

	CurrentVelocity += FrictionAcceleration * DeltaTime;

	if (FMath::Abs(Speed) <= FMath::Abs(FrictionAcceleration.Size() * DeltaTime))
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
	FVector RandomOffset = FMath::VRandCone(Direction, FMath::DegreesToRadians(RandomAngle));

	return RandomOffset;
}

float ATankCharacter::GetAngleOfRandomOffset()
{
	// 获取鼠标保持不动的时间
	float IdleTime = GetWorld()->GetTimeSeconds() - LastMouseMoveTime;

	// 根据鼠标保持不动的时间进行插值计算
	float Alpha = IdleTime / ShrinkTime;
	Alpha = FMath::Min(Alpha, 1.0f);

	return FMath::Lerp(MaxAngle, MinAngle, Alpha);
}

void ATankCharacter::SetMouseMoveTime()
{
	LastMouseMoveTime = GetWorld()->GetTimeSeconds();
}

void ATankCharacter::Fire()
{
	FVector LaunchDirection = CalculateLaunchDirection();
	if (LaunchDirection.Length() < 1e-3) return;

	// 创建炮弹
	if (!ProjectileClass) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (ATankProjectile* Projectile = GetWorld()->SpawnActor<ATankProjectile>(
		ProjectileClass, FirePosition->GetComponentLocation(), LaunchDirection.Rotation(), SpawnParams))
	{
		LaunchDirection = AddRandomOffset(LaunchDirection, GetAngleOfRandomOffset());
		Projectile->Launch(LaunchDirection);
		// log: GetAngleOfRandomOffset()
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		                                 FString::Printf(TEXT("GetAngleOfRandomOffset: %f"), GetAngleOfRandomOffset()));
	}
}

// 计算炮弹的发射方向
FVector ATankCharacter::CalculateLaunchDirection() const
{
	// 获取鼠标位置下的碰撞点
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	// 计算炮弹的发射方向
	FVector LaunchDirection = FVector::ZeroVector;
	if (HitResult.bBlockingHit)
	{
		LaunchDirection = HitResult.ImpactPoint - FirePosition->GetComponentLocation();
		LaunchDirection.Normalize();
	}

	return LaunchDirection;
}

void ATankCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 旋转相机
	PlayerInputComponent->BindAxis("Turn", this, &ATankCharacter::TurnTurret);
	PlayerInputComponent->BindAxis("LookUp", this, &ATankCharacter::CameraUpAndDown);

	// 移动
	PlayerInputComponent->BindAxis("MoveForward", this, &ATankCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATankCharacter::MoveRight);

	// 开火事件
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATankCharacter::Fire);
}
