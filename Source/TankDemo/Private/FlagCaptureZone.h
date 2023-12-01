// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagCaptureZone.generated.h"

UCLASS(Abstract)  // set collider in derived blueprint
class AFlagCaptureZone : public AActor {
  GENERATED_BODY()

 public:
  AFlagCaptureZone();

  virtual void BeginPlay() override;

  virtual void Tick(float DeltaTime) override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite,
            Category = "Flag Capture Settings")
  UStaticMeshComponent* ZoneMesh;  // 可视化夺旗区域的网格组件

  UPROPERTY(EditAnywhere, Category = "Flag Capture Settings")
  FString ZoneName;  // 区域名称

  UPROPERTY(EditAnywhere, Category = "Flag Capture Settings")
  TArray<ACharacter*> Team1Players;  // 队伍1玩家

  UPROPERTY(EditAnywhere, Category = "Flag Capture Settings")
  TArray<ACharacter*> Team2Players;  // 队伍2玩家

  UPROPERTY(EditAnywhere, Category = "Flag Capture Settings")
  float CaptureProgressThreshold;  // 夺旗进度阈值

  UFUNCTION(BlueprintPure, Category = "Flag Capture Settings")
  float GetCaptureProgress();  // 获取夺旗进度

  UPROPERTY(EditAnywhere, BlueprintReadWrite,
            Category = "Flag Capture Settings")
  float CaptureTime;

 protected:
  UFUNCTION()
  void OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                          AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex, bool bFromSweep,
                          const FHitResult& SweepResult);

  UFUNCTION()
  void OnZoneEndOverlap(UPrimitiveComponent* OverlappedComponent,
                        AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex);

 private:
  float Team1Progress;  // 队伍1夺旗进度
  float Team2Progress;  // 队伍2夺旗进度
};
