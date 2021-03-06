/************************************************************************/
/* UMBRA TOOLS                                                          */
/* Maxwell - Axel Clerget   											*/
/************************************************************************/

#include "GameEventManager.h"
#include "Object/GameEventContainerObject.h"
#include "Object/GameEvent.h"
#include "Engine.h"

AGameEventManager* AGameEventManager::GameEventManagerInstance = nullptr;

AGameEventManager::AGameEventManager(const FObjectInitializer& Obj) : Super(Obj)
{
	PrimaryActorTick.bCanEverTick = false;

	this->GameplayTaskComponent = Obj.CreateDefaultSubobject<UGameplayTasksComponent>(this, FName("Gameplay Task Component"));
}

void AGameEventManager::Init()
{
	if (this->GameEventContainer == nullptr)
	{
		this->GameEventContainer = NewObject<UGameEventContainerObject>();
		if (this->GameEventContainer)
			this->GameEventContainer->Init(this->GameEventContainerBase, this);
	}
}

// Called when the game starts or when spawned
void AGameEventManager::BeginPlay()
{
	Super::BeginPlay();	

	this->Init();
}

void AGameEventManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	this->GameEventContainer->ConditionalBeginDestroy();
	this->GameEventContainer = nullptr;
	if (GEngine)
		GEngine->ForceGarbageCollection(true);

	/* Clean pointer reference */
	AGameEventManager::GameEventManagerInstance = nullptr;

	Super::EndPlay(EndPlayReason);
}

bool AGameEventManager::TryActivateEvents(FGameplayTagContainer EventsTags)
{
	this->Init();

	if (this->GameEventContainer == nullptr)
		return false;

	bool HasActivatedEvents = false;
	for (UGameEvent* const& GameEvent : this->GameEventContainer->GameEvents)
	{
		if (GameEvent->MatchEventsTags(EventsTags))
		{
			bool HasActivated = GameEvent->TryActivate();

			if (HasActivated)
			{
				this->TryCancelEvents(EventsTags);
				HasActivatedEvents = true;
			}
		}
	}

	return HasActivatedEvents;
}

UGameEvent* AGameEventManager::GetGameEventById(FGuid Id)
{
	if (this->GameEventContainer)
	{
		for (UGameEvent* const& GameEvent : this->GameEventContainer->GameEvents)
		{
			if (GameEvent->Id == Id && GameEvent->IsActive())
				return GameEvent;
		}
	}

	return nullptr;
}

UGameEvent* AGameEventManager::GetGameEventByKey(FString Key)
{
	if (this->GameEventContainer)
	{
		for (UGameEvent* const& GameEvent : this->GameEventContainer->GameEvents)
		{
			if (GameEvent->Key == Key && GameEvent->IsActive())
				return GameEvent;
		}
	}

	return nullptr;
}

TArray<UGameEvent*> AGameEventManager::GetActiveGameEvents()
{
	TArray<UGameEvent*> GameEvents;
	if (this->GameEventContainer)
	{
		for (UGameEvent* const& GameEvent : this->GameEventContainer->GameEvents)
		{
			if (GameEvent->IsActive())
				GameEvents.Add(GameEvent);
		}
	}

	return GameEvents;
}

TArray<UGameEvent*> AGameEventManager::GetCompleteGameEvents()
{
	TArray<UGameEvent*> GameEvents;
	if (this->GameEventContainer)
	{
		for (UGameEvent* const& GameEvent : this->GameEventContainer->GameEvents)
		{
			if (GameEvent->IsComplete())
				GameEvents.Add(GameEvent);
		}
	}

	return GameEvents;
}

TArray<UGameEvent*> AGameEventManager::GetCancelGameEvents()
{
	TArray<UGameEvent*> GameEvents;
	if (this->GameEventContainer)
	{
		for (UGameEvent* const& GameEvent : this->GameEventContainer->GameEvents)
		{
			if (GameEvent->IsCancel())
				GameEvents.Add(GameEvent);
		}
	}

	return GameEvents;
}

void AGameEventManager::AddCustomTagsToEvent(FGameplayTagContainer CustomTags, UGameEvent* GameEvent)
{
	if (GameEvent)
		GameEvent->AddCustomTags(CustomTags);
}

void AGameEventManager::RemoveCustomTagsToEvent(FGameplayTagContainer CustomTags, UGameEvent* GameEvent)
{
	if (GameEvent)
		GameEvent->RemoveCustomTags(CustomTags);
}

void AGameEventManager::TryCancelEvents(FGameplayTagContainer EventsTags)
{
	for (UGameEvent* const& GameEvent : this->GameEventContainer->GameEvents)
	{
 		if (GameEvent->IsActive())
 			GameEvent->TryCancelEvent(EventsTags);
	}
}
