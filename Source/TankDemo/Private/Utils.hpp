#pragma once

#include "CoreMinimal.h"

inline void MyLog(const FString& InString)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, InString);
}

inline void MyLogWarning(const FString& InString)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, InString);
}

inline void MyLogError(const FString& InString)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, InString);
}
