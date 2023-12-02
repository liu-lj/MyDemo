// ReSharper disable IdentifierTypo
#pragma once

#include "CoreMinimal.h"

inline void MyLog(const FString& InString)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, InString);
}

template <typename FmtType, typename... Types>
inline static void MyLogf(const FmtType& Fmt, Types... Args)
{
	MyLog(FString::Printf(Fmt, Args...));
}

inline void MyLogWarning(const FString& InString)
{
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, InString);
}

template <typename FmtType, typename... Types>
inline static void MyLogWarningf(const FmtType& Fmt, Types... Args)
{
	MyLogWarning(FString::Printf(Fmt, Args...));
}

inline void MyLogError(const FString& InString)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, InString);
}

template <typename FmtType, typename... Types>
inline static void MyLogErrorf(const FmtType& Fmt, Types... Args)
{
	MyLogError(FString::Printf(Fmt, Args...));
}