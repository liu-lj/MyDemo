// Demo by LLJ. You just DO WHAT YOU WANT TO.

#include "TankComponent.h"
#include "Utils.hpp"

UTankComponent::UTankComponent():
	bIsDestroyed(false),
	bIsLoaded(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankComponent::Destroy()
{
	DestroyedTime = GetWorld()->GetTimeSeconds();
	if (!bIsDestroyed)
	{
		MyOnComponentDestroyed();
		bIsDestroyed = true;
	}
	GetWorld()->GetTimerManager().SetTimer(RepairTimerHandle, this, &UTankComponent::TryRepairComponent, RepairTime,
	                                       false);
	OnComponentDestroyed.Broadcast();
}

bool UTankComponent::IsUsable() const
{
	return !bIsDestroyed;
}

void UTankComponent::Load()
{
	bIsLoaded = true;
	OnComponentLoaded.Broadcast();
}

void UTankComponent::Unload()
{
	bIsLoaded = false;
	OnComponentUnloaded.Broadcast();
}

bool UTankComponent::IsLoaded() const
{
	return bIsLoaded;
}

void UTankComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTankComponent::TryRepairComponent()
{
	float TimeNow = GetWorld()->GetTimeSeconds();
	if (TimeNow - DestroyedTime >= RepairTime)
	{
		bIsDestroyed = false;
		MyLog("component repaired");
		MyOnComponentRepaired();
		OnComponentRepaired.Broadcast();
	}
}
