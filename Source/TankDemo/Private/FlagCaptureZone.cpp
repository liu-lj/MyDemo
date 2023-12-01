// Demo by LLJ. You just DO WHAT YOU WANT TO.

#include "FlagCaptureZone.h"

#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Utils.hpp"

AFlagCaptureZone::AFlagCaptureZone() {
  // MyLog("AFlagCaptureZone::AFlagCaptureZone");
  PrimaryActorTick.bCanEverTick = false;

  CaptureProgressThreshold = 0.5f;

  ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
  RootComponent = ZoneMesh;
}

void AFlagCaptureZone::OnZoneBeginOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult) {
  // 检查碰撞体是否是玩家
  ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
  if (PlayerCharacter) {
    MyLog(FString::Printf(TEXT("PlayerCharacter now enter zone: %s"),
                          *PlayerCharacter->GetName()));
    // 记录玩家进入夺旗区域的时间
    float EnterTime = GetWorld()->GetTimeSeconds();
    // 在这里你可以将进入时间保存到玩家角色的变量中，或者在AFlagCaptureZone类中创建一个映射（Map）来保存玩家进出时间等信息
  } else
    MyLogWarning(TEXT("other is not a character"));
}

void AFlagCaptureZone::OnZoneEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  // 检查碰撞体是否是玩家
  ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
  if (PlayerCharacter) {
    MyLog(FString::Printf(TEXT("PlayerCharacter now leave zone: %s"),
                          *PlayerCharacter->GetName()));
    // 记录玩家离开夺旗区域的时间
    float LeaveTime = GetWorld()->GetTimeSeconds();
    // 在这里你可以将离开时间保存到玩家角色的变量中，或者在AFlagCaptureZone类中创建一个映射（Map）来保存玩家进出时间等信息
  } else
    MyLogWarning(TEXT("other is not a character"));
}

void AFlagCaptureZone::BeginPlay() { Super::BeginPlay(); }

float AFlagCaptureZone::GetCaptureProgress() {
  int32 Team1Count = Team1Players.Num();
  int32 Team2Count = Team2Players.Num();

  if (int32 TotalPlayers = Team1Count + Team2Count; TotalPlayers > 0) {
    Team1Progress =
        static_cast<float>(Team1Count) / static_cast<float>(TotalPlayers);
    Team2Progress =
        static_cast<float>(Team2Count) / static_cast<float>(TotalPlayers);
  }

  return FMath::Abs(Team1Progress - Team2Progress);
}

void AFlagCaptureZone::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (float Progress = GetCaptureProgress();
      Progress >= CaptureProgressThreshold) {
    // 夺旗成功逻辑
  }
}
