// Demo by LLJ. You just DO WHAT YOU WANT TO.


#include "SoundManager.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ASoundManager::ASoundManager()
{
	PrimaryActorTick.bCanEverTick = false;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
}

ASoundManager* ASoundManager::Instance = nullptr;
ASoundManager* ASoundManager::GetInstance()
{
	return Instance;
}

void ASoundManager::PlayRandomSound(TArray<USoundBase*> Sounds) const
{
	// 随机播放语音
	if (Sounds.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, Sounds.Num() - 1);
		if (Sounds[RandomIndex])
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sounds[RandomIndex], GetActorLocation());
		}
	}
}

#define IMPL_PLAY_RANDOM_SOUND_FUNC(Name) \
	void ASoundManager::Play##Name##Sound() \
	{ \
		PlayRandomSound(Name##Sounds); \
	}

IMPL_PLAY_RANDOM_SOUND_FUNC(Fire)
IMPL_PLAY_RANDOM_SOUND_FUNC(Hit)
IMPL_PLAY_RANDOM_SOUND_FUNC(Ricochet)
IMPL_PLAY_RANDOM_SOUND_FUNC(GunDamaged)
IMPL_PLAY_RANDOM_SOUND_FUNC(TurretDamaged)
IMPL_PLAY_RANDOM_SOUND_FUNC(TrackDamaged)
IMPL_PLAY_RANDOM_SOUND_FUNC(TrackRepaired)
IMPL_PLAY_RANDOM_SOUND_FUNC(Fail)
IMPL_PLAY_RANDOM_SOUND_FUNC(Destroyed)
IMPL_PLAY_RANDOM_SOUND_FUNC(Start)

void ASoundManager::BeginPlay()
{
	Super::BeginPlay();

	// 设置为单例类，只保留一个实例
	if (GetWorld())
	{
		TArray<AActor*> SoundEffectActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoundManager::StaticClass(), SoundEffectActors);
		if (SoundEffectActors.Num() > 1)
		{
			for (int32 i = 1; i < SoundEffectActors.Num(); i++)
			{
				SoundEffectActors[i]->Destroy();
			}
		}
		Instance = Cast<ASoundManager>(SoundEffectActors[0]);
	}
}
