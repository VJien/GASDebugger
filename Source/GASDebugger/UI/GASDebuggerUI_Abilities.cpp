// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Abilities.h"

#include "AbilitySystemComponent.h"
#include "GASDebugger/GASDebuggerLibrary.h"
#include "GASDebugger/GASDebuggerSettings.h"
#include "GASDebugger/Demo/GASDebuggerLogger.h"
#include "Kismet/KismetMathLibrary.h"

void UGASDebuggerUI_Abilities::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	Super::InitAbilityWidget_Implementation(AbilitySystemComponent);
	RemovedAbilities.Empty();
	CurrentAbilities.Empty();

	if (AbilitySystemComponent)
	{
		// Use polling for Give/Remove, but delegates for Activate/End as they are reliable
		AbilitySystemComponent->AbilityActivatedCallbacks.AddUObject(this, &UGASDebuggerUI_Abilities::HandleAbilityActivated);
		AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UGASDebuggerUI_Abilities::HandleAbilityEnded);
	}
}


void UGASDebuggerUI_Abilities::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateAbilities(); // We now use polling
}

void UGASDebuggerUI_Abilities::UpdateAbilities()
{
	if (OwningAbilitySystemComponent == nullptr)
	{
		return;
	}

	// 1. Get the latest list of ability specs
	const TArray<FGameplayAbilitySpec>& LatestAbilitySpecs = OwningAbilitySystemComponent->GetActivatableAbilities();
	TArray<UGameplayAbility*> NewAbilities;
	for (const FGameplayAbilitySpec& Spec : LatestAbilitySpecs)
	{
		if(Spec.Ability)
		{
			NewAbilities.Add(Spec.Ability);
		}
	}

	// 2. Only process if the list has changed
	if (NewAbilities != CurrentAbilities)
	{
		// 3. Find removed abilities and log them
		TArray<UGameplayAbility*> RemovedList = CurrentAbilities; // Copy old list for comparison
		RemovedList.RemoveAll([&NewAbilities](UGameplayAbility* Ability)
		{
			return NewAbilities.Contains(Ability);
		});

		for (UGameplayAbility* RemovedAbility : RemovedList)
		{
			if (UGASDebuggerSettings::Get()->bEnableLogging && RemovedAbility)
			{
				FString LogMessage = FString::Printf(TEXT("%s - Ability Removed: %s "), *FDateTime::Now().ToString(), *RemovedAbility->GetName());
				FGASDebuggerLogger::Log(ELogCategory::Abilities, LogMessage);
			}
		}

		// 4. Find newly added abilities and log them
		for (UGameplayAbility* NewAbility : NewAbilities)
		{
			if (!CurrentAbilities.Contains(NewAbility)) // If the old list doesn't contain this new ability, it was just added
			{
				if (UGASDebuggerSettings::Get()->bEnableLogging && NewAbility)
				{
					FString LogMessage = FString::Printf(TEXT("%s - Ability Given: %s "), *FDateTime::Now().ToString(), *NewAbility->GetName());
					FGASDebuggerLogger::Log(ELogCategory::Abilities, LogMessage);
				}
			}
		}
		
		// 5. Update UI widgets
		TArray<FGASDebuggerUI_AbilitiesData> RemovedAbilitiesData;
		for (auto&& RemoveAbility : RemovedList)
		{
			FGASDebuggerUI_AbilitiesData Data;
			Data.Ability = RemoveAbility;
			Data.Info = UGASDebuggerLibrary::GetTimeInfo();
			RemovedAbilitiesData.Add(Data);
		}
		RefreshRemovedAbilitiesWidget(RemovedAbilitiesData);
		RefreshCurrentAbilitiesWidget(NewAbilities);

		// 6. Finally, update the current list for the next frame
		CurrentAbilities = NewAbilities;
	}
}

void UGASDebuggerUI_Abilities::HandleGiveAbility(FGameplayAbilitySpec& Spec)
{
	// This function is no longer used due to polling method
}

void UGASDebuggerUI_Abilities::HandleRemoveAbility(FGameplayAbilitySpec& Spec)
{
	// This function is no longer used due to polling method
}

void UGASDebuggerUI_Abilities::HandleAbilityActivated(UGameplayAbility* Ability)
{
	if (UGASDebuggerSettings::Get()->bEnableLogging && Ability)
	{
		FString LogMessage = FString::Printf(TEXT("%s - Ability Activated: %s"), *FDateTime::Now().ToString(), *Ability->GetName());
		FGASDebuggerLogger::Log(ELogCategory::Abilities, LogMessage);
	}
}

void UGASDebuggerUI_Abilities::HandleAbilityEnded( const FAbilityEndedData& Data)
{
	if (UGASDebuggerSettings::Get()->bEnableLogging)
	{
		FString AbilityName = Data.AbilityThatEnded ? Data.AbilityThatEnded->GetName() : TEXT("Unknown Ability");
		FString LogMessage = FString::Printf(TEXT("%s - Ability Ended: %s"), *FDateTime::Now().ToString(), *AbilityName);
		FGASDebuggerLogger::Log(ELogCategory::Abilities, LogMessage);
	}
}
