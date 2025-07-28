// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Abilities.h"

#include "AbilitySystemComponent.h"
#include "GASDebugger/GASDebuggerLibrary.h"
#include "Kismet/KismetMathLibrary.h"

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
		TArray<FGASDebuggerUI_AbilitiesData> RemovedAbilitiesData;
		for (auto&& RemoveAbility : RemovedAbilities)
		{
			FGASDebuggerUI_AbilitiesData Data;
			Data.Ability = RemoveAbility;
			Data.Info = UGASDebuggerLibrary::GetTimeInfo();
			RemovedAbilitiesData.Add(Data);
		}
		RefreshRemovedAbilitiesWidget(RemovedAbilitiesData);
		RefreshCurrentAbilitiesWidget(CurrentAbilities);
	}
		
	
}
