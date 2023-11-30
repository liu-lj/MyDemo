// Demo by LLJ. You just DO WHAT YOU WANT TO.

#include "FlagCaptureZone.h"

#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

void MyLogWarning(const FString& InString)
{
	// return;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, InString);
}

void MyLog(const FString& InString)
{
	// return;
	UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, InString);
}

void PrintCollisionInfo(const UStaticMeshComponent* smComp)
{
	MyLog(TEXT("PrintCollisionInfo"));
	if (!smComp)
	{
		MyLogWarning(TEXT("SMesh is NULL"));
		return;
	}

	MyLog(FString::Printf(TEXT("SMesh: %s"), *smComp->GetName()));

	if (UStaticMesh* Mesh = smComp->GetStaticMesh())
	{
		MyLog(FString::Printf(TEXT("Static Mesh Name: %s"), *Mesh->GetName()));

		// 获取碰撞设置
		ECollisionEnabled::Type CollisionType = smComp->GetCollisionEnabled();
		MyLog(FString::Printf(TEXT("Collision Type: %d"), static_cast<int32>(CollisionType)));
		
		const FCollisionShape CollisionShape = smComp->GetCollisionShape();
		auto ShapeType = CollisionShape.ShapeType;
		MyLog(FString::Printf(TEXT("Collision Shape Type: %d"), static_cast<int32>(ShapeType)));

		// 获取碰撞响应
		FCollisionResponseContainer CollisionResponse = smComp->GetCollisionResponseToChannels();
		// UE_LOG(LogTemp, Warning, TEXT("Collision Response: %s"), CollisionResponse->ToString());

		// 获取是否启用物理模拟
		bool bSimulatePhysics = smComp->IsSimulatingPhysics();
		MyLog(FString::Printf(TEXT("Simulate Physics: %d"), bSimulatePhysics));
	}
	else MyLogWarning(TEXT("GetStaticMesh is NULL"));	
}

AFlagCaptureZone::AFlagCaptureZone()
{
	MyLog("AFlagCaptureZone::AFlagCaptureZone");
	PrimaryActorTick.bCanEverTick = false;

	CaptureProgressThreshold = 0.5f;

	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
	RootComponent = ZoneMesh;
}

void AFlagCaptureZone::OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
										  const FHitResult& SweepResult)
{
	// 检查碰撞体是否是玩家
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		MyLog(FString::Printf(TEXT("PlayerCharacter now enter zone: %s"), *PlayerCharacter->GetName()));
		// 记录玩家进入夺旗区域的时间
		float EnterTime = GetWorld()->GetTimeSeconds();
		// 在这里你可以将进入时间保存到玩家角色的变量中，或者在AFlagCaptureZone类中创建一个映射（Map）来保存玩家进出时间等信息
	}
	else MyLogWarning(TEXT("other is not a character"));
}

void AFlagCaptureZone::OnZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
										int32 OtherBodyIndex)
{
	// 检查碰撞体是否是玩家
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		MyLog(FString::Printf(TEXT("PlayerCharacter now leave zone: %s"), *PlayerCharacter->GetName()));
		// 记录玩家离开夺旗区域的时间
		float LeaveTime = GetWorld()->GetTimeSeconds();
		// 在这里你可以将离开时间保存到玩家角色的变量中，或者在AFlagCaptureZone类中创建一个映射（Map）来保存玩家进出时间等信息
	}
	else MyLogWarning(TEXT("other is not a character"));
}

void AFlagCaptureZone::BeginPlay()
{
	Super::BeginPlay();

	PrintCollisionInfo(ZoneMesh);
}

float AFlagCaptureZone::GetCaptureProgress()
{
	int32 Team1Count = Team1Players.Num();
	int32 Team2Count = Team2Players.Num();

	if (int32 TotalPlayers = Team1Count + Team2Count; TotalPlayers > 0)
	{
		Team1Progress = static_cast<float>(Team1Count) / static_cast<float>(TotalPlayers);
		Team2Progress = static_cast<float>(Team2Count) / static_cast<float>(TotalPlayers);
	}

	return FMath::Abs(Team1Progress - Team2Progress);
}

void AFlagCaptureZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (float Progress = GetCaptureProgress(); Progress >= CaptureProgressThreshold)
	{
		// 夺旗成功逻辑
	}
}
