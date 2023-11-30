// Demo by LLJ. You just DO WHAT YOU WANT TO.


#include "TankCharacter.h"

ATankCharacter::ATankCharacter() :
	MovementSpeed(500.0f),
	RotationSpeed(100.0f),
	MaxSpeed(1000.0f),
	Acceleration(10.0f),
	Friction(200.0f)
{
	// 创建车身网格组件
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);

	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(BodyMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(CameraRoot);

	// 创建炮塔网格组件
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	// 创建炮管网格组件
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BarrelMesh->SetupAttachment(TurretMesh);
}

void ATankCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// 实现MoveForward函数，根据输入的值来设置角色的前后移动
void ATankCharacter::MoveForward(float AxisValue)
{
	FVector Direction = GetActorForwardVector();
	FVector Force = Direction * AxisValue * Acceleration;

	CurrentVelocity += Force;
}

// 实现MoveRight函数，根据输入的值来设置角色的左右移动
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
}

void ATankCharacter::CameraUpAndDown(float AxisValue)
{
	float RotationChange = AxisValue * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator CameraRotation = Camera->GetRelativeRotation();
	CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch + RotationChange, -50.0f, 20.0f);
	Camera->SetRelativeRotation(CameraRotation);
}

void ATankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyFriction(DeltaTime);
}

// 实现ApplyFriction函数，根据摩擦力来减缓速度
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

void ATankCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 绑定鼠标旋转事件
	PlayerInputComponent->BindAxis("Turn", this, &ATankCharacter::TurnTurret);
	PlayerInputComponent->BindAxis("LookUp", this, &ATankCharacter::CameraUpAndDown);

	// 绑定 WASD 键事件
	PlayerInputComponent->BindAxis("MoveForward", this, &ATankCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATankCharacter::MoveRight);
}
