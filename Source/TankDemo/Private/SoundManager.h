// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "SoundManager.generated.h"

UCLASS()
class ASoundManager : public AActor
{
	GENERATED_BODY()

public:
	ASoundManager();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	static ASoundManager* GetInstance();

	// 开炮
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> FireSounds;
	void PlayFireSound();
	
	// 击中
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> HitSounds;
	void PlayHitSound();
	
	// 跳弹
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> RicochetSounds;
	void PlayRicochetSound();
	
	// 火炮坏
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> GunDamagedSounds;
	void PlayGunDamagedSound();
	
	// 炮塔坏
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> TurretDamagedSounds;
	void PlayTurretDamagedSound();
	
	// 履带坏
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> TrackDamagedSounds;
	void PlayTrackDamagedSound();
	
	// 履带修复
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> TrackRepairedSounds;
	void PlayTrackRepairedSound();
	
	// 失败
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> FailSounds;
	void PlayFailSound();
	
	// 击毁
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> DestroyedSounds;
	void PlayDestroyedSound();
	
	// 出发
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> StartSounds;
	void PlayStartSound();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;

	static ASoundManager* Instance;

	void PlayRandomSound(TArray<USoundBase*> Sounds) const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
