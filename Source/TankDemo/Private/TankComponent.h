// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankComponent.generated.h"

UENUM(BlueprintType)
enum class EComponentState : uint8
{
	Normal,
	Damaged
};

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTankComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTankComponent();

	UFUNCTION(BlueprintCallable, Category = "Component")
	// 部件损坏
	void Destroy();

	UFUNCTION(BlueprintCallable, Category = "Component")
	// 部件是否可以使用
	bool IsUsable() const;

	virtual void BeginPlay() override;

protected:
	EComponentState ComponentState;

	UPROPERTY(EditAnywhere, Category = "Component")
	float RepairTime;

	UFUNCTION(BlueprintImplementableEvent, Category = "Component")
	void OnComponentDamaged();

	UFUNCTION(BlueprintImplementableEvent, Category = "Component")
	void OnComponentRepaired();

private:
	float DestroyedTime;
	FTimerHandle RepairTimerHandle;

	void TryRepairComponent();
};
