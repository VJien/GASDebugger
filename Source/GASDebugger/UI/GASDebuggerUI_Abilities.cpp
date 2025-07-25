// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Abilities.h"

#include "AbilitySystemComponent.h"

void UGASDebuggerUI_Abilities::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	Super::InitAbilityWidget_Implementation(AbilitySystemComponent);
	RemovedAbilities.Empty();
	CurrentAbilities.Empty();
}


void UGASDebuggerUI_Abilities::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateAbilities();
}

void UGASDebuggerUI_Abilities::UpdateAbilities()
{
	if (OwningAbilitySystemComponent == nullptr)
	{
		return;
	}
	TArray<UGameplayAbility*> NewAbilities;
	auto&& Abilities =OwningAbilitySystemComponent->GetActivatableAbilities();
	for (auto&& Ability:Abilities)
	{
		NewAbilities.Add(Ability.Ability);
	}
	if (NewAbilities != CurrentAbilities)
	{
		RemovedAbilities = CurrentAbilities;
		RemovedAbilities.RemoveAll([&NewAbilities](UGameplayAbility* Ability)
		{
			return NewAbilities.Contains(Ability);
		});
		CurrentAbilities = NewAbilities;
		RefreshRemovedAbilitiesWidget(RemovedAbilities);
		RefreshCurrentAbilitiesWidget(CurrentAbilities);
	}
		
	
}
