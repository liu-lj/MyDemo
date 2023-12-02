// Demo by LLJ. You just DO WHAT YOU WANT TO.


#include "UEventManager.h"

UEventManager* UEventManager::Instance = nullptr;

UEventManager* UEventManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UEventManager>();
	}

	return Instance;
}

void UEventManager::TriggerMyEvent() const
{
	OnMyEvent.Broadcast();
}
