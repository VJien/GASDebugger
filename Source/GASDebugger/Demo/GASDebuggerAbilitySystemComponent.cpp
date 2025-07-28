// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerAbilitySystemComponent.h"


// Sets default values for this component's properties
UGASDebuggerAbilitySystemComponent::UGASDebuggerAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGASDebuggerAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGASDebuggerAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FGameplayAbilitySpecHandle UGASDebuggerAbilitySystemComponent::AddNewAbility(TSubclassOf<UGameplayAbility> AbilityClass,
	int32 level)
{
	FGameplayAbilitySpecHandle SpecHandle;
	if (!AbilityClass)
	{
		return SpecHandle;
	}
	auto&& AbilityCDO = AbilityClass->GetDefaultObject<UGameplayAbility>();
	FGameplayAbilitySpec AbilitySpec(AbilityCDO, level);
	SpecHandle = GiveAbility(AbilitySpec);
	return SpecHandle;
}

bool UGASDebuggerAbilitySystemComponent::RemoveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass,
                                                              FGameplayAbilitySpecHandle& OutHandle)
{
	if (FGameplayAbilitySpec* spec = FindAbilitySpecFromClass(AbilityClass))
	{
		OutHandle = spec->Handle;
		this->ClearAbility(spec->Handle);
		return true;
	}
	return false;
}

void UGASDebuggerAbilitySystemComponent::AddTag(FGameplayTag Tag, int32 Count)
{
	AddLooseGameplayTag(Tag,Count);
}

void UGASDebuggerAbilitySystemComponent::AddTags(FGameplayTagContainer TagContainer)
{
	AddLooseGameplayTags(TagContainer);
}

void UGASDebuggerAbilitySystemComponent::RemoveTag(FGameplayTag Tag, int32 Count)
{
	RemoveLooseGameplayTag(Tag,Count);
}

void UGASDebuggerAbilitySystemComponent::RemoveTags(FGameplayTagContainer TagContainer)
{
	RemoveLooseGameplayTags(TagContainer);
}
