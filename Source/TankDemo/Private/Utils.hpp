#pragma once

#include "CoreMinimal.h"

void MyLogWarning(const FString& InString) {
  UE_LOG(LogTemp, Warning, TEXT("%s"), *InString);
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, InString);
}

void MyLog(const FString& InString) {
  UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, InString);
}