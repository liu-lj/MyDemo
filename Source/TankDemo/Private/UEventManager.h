// Demo by LLJ. You just DO WHAT YOU WANT TO.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UEventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyEventDelegate);

UCLASS()
class UEventManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	static UEventManager* GetInstance();

	UPROPERTY(BlueprintAssignable)
	FMyEventDelegate OnMyEvent;

	UFUNCTION(BlueprintCallable, Category = "Event")
	void TriggerMyEvent() const;

private:
	static UEventManager* Instance;
};