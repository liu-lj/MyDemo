// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableTank.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDamageableTank : public UInterface
{
	GENERATED_BODY()
};

class IDamageableTank
{
	GENERATED_BODY()

public:
	virtual float TankTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                             AActor* DamageCauser, float InPenetrationAngle, float InPenetrationDepth,
	                             UPrimitiveComponent* HitComponent) = 0;
};
