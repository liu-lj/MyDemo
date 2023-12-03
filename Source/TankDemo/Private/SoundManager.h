// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayFireSound();
	
	// 击中
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> HitSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayHitSound();
	
	// 跳弹
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> RicochetSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayRicochetSound();
	
	// 火炮坏
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> GunDamagedSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayGunDamagedSound();
	
	// 炮塔坏
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> TurretDamagedSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayTurretDamagedSound();
	
	// 履带坏
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> TrackDamagedSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayTrackDamagedSound();
	
	// 履带修复
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> TrackRepairedSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayTrackRepairedSound();
	
	// 失败
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> FailSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayFailSound();
	
	// 击毁
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> DestroyedSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayDestroyedSound();
	
	// 出发
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> StartSounds;
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayStartSound();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;

	static ASoundManager* Instance;

	void PlayRandomSound(TArray<USoundBase*> Sounds) const;

protected:
	virtual void BeginPlay() override;
};
