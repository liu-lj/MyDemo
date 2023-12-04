// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentRepaired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentUnloaded);

UENUM(BlueprintType)
enum class ETankComponentType : uint8
{
	Base UMETA(DisplayName = "Base"),
	Additional UMETA(DisplayName = "Additional")
};

/*
 * 部件类
 * 1. 基础部件：
 *	  包括炮塔、炮管、履带
 *	  有正常和损坏两种状态，损坏后经过一段时间可以修复
 * 2. 增益部件：
 *    包括输弹机、火控系统、发动机
 *    有使用和闲置两种状态
 */
UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTankComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTankComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	ETankComponentType Type;

	UFUNCTION(BlueprintCallable, Category = "Component")
	// 部件损坏
	void Destroy();
	UPROPERTY(BlueprintAssignable, Category = "Component")
	FOnComponentDestroyed OnComponentDestroyed;
	UPROPERTY(BlueprintAssignable, Category = "Component")
	FOnComponentRepaired OnComponentRepaired;

	UFUNCTION(BlueprintCallable, Category = "Component")
	// 部件是否可以使用
	bool IsUsable() const;

	UFUNCTION(BlueprintCallable, Category = "Component")
	// 使用部件
	void Load();
	UPROPERTY(BlueprintAssignable, Category = "Component")
	FOnComponentLoaded OnComponentLoaded;

	UFUNCTION(BlueprintCallable, Category = "Component")
	// 停止使用部件
	void Unload();
	UPROPERTY(BlueprintAssignable, Category = "Component")
	FOnComponentUnloaded OnComponentUnloaded;

	UFUNCTION(BlueprintCallable, Category = "Component")
	// 部件是否在使用中
	bool IsLoaded() const;

	virtual void BeginPlay() override;

protected:
	bool bIsDestroyed;
	bool bIsLoaded;

	UPROPERTY(EditAnywhere, Category = "Component")
	float RepairTime;

	UFUNCTION(BlueprintImplementableEvent, Category = "Component")
	void MyOnComponentDestroyed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Component")
	void MyOnComponentRepaired();

private:
	float DestroyedTime;
	FTimerHandle RepairTimerHandle;

	void TryRepairComponent();
};
