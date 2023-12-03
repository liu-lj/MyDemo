// Demo by LLJ. You just DO WHAT YOU WANT TO.

#include "TankComponent.h"
#include "Utils.hpp"

UTankComponent::UTankComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankComponent::Destroy()
{
	DestroyedTime = GetWorld()->GetTimeSeconds();
	if (ComponentState == EComponentState::Normal)
	{
		OnComponentDamaged();
		ComponentState = EComponentState::Damaged;
	}
	GetWorld()->GetTimerManager().SetTimer(RepairTimerHandle, this, &UTankComponent::TryRepairComponent, RepairTime,
	                                       false);
}

bool UTankComponent::IsUsable() const
{
	return ComponentState == EComponentState::Normal;
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
		ComponentState = EComponentState::Normal;
		MyLog("component repaired");
		OnComponentRepaired();
	}
}
