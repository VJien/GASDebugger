// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_GE.h"
#include "AbilitySystemComponent.h"
#include "GASDebugger/GASDebuggerSettings.h"
#include "GASDebugger/Demo/GASDebuggerLogger.h"

void UGASDebuggerUI_GE::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	Super::InitAbilityWidget_Implementation(AbilitySystemComponent);
}

void UGASDebuggerUI_GE::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (OwningAbilitySystemComponent == nullptr)
	{
		return;
	}
	
	// Get current effects based on engine version
#if UE5_OR_LATER
	const FActiveGameplayEffectsContainer& GeContainer = OwningAbilitySystemComponent->GetActiveGameplayEffects();
	CurrActiveGameplayEffects = GeContainer.GetAllActiveEffectHandles();
#else
	// This is a simplified way for older engines, might need adjustment based on specific 4.x version
	FGameplayEffectQuery Query;
	Query.EffectSource = nullptr;
	CurrActiveGameplayEffects = OwningAbilitySystemComponent->GetActiveEffects(Query);
#endif
	
	if (CurrActiveGameplayEffects.Num() == 0 && LastActiveGameplayEffects.Num() == 0)
	{
		return;
	}
	
	// Compare and find differences
	CompareGEs(CurrActiveGameplayEffects, RemovedGameplayEffects, AppliedGameplayEffects);
	
	// Update the list for the next frame
	LastActiveGameplayEffects = CurrActiveGameplayEffects;

	// Notify blueprints if there are changes
	if (AppliedGameplayEffects.Num() > 0)
	{
		OnGameplayEffectApplied(AppliedGameplayEffects);
	}
	if (RemovedGameplayEffects.Num() > 0)
	{
		OnGameplayEffectRemoved(RemovedGameplayEffects);
	}
}

void UGASDebuggerUI_GE::CompareGEs(const TArray<FActiveGameplayEffectHandle>& Current, TArray<FActiveGameplayEffectHandle>& Removed, TArray<FActiveGameplayEffectHandle>& Applied)
{
	Removed.Empty();
	Applied.Empty();

	// Find removed effects
	for (const FActiveGameplayEffectHandle& LastGEHandle : LastActiveGameplayEffects)
	{
		if (!Current.Contains(LastGEHandle))
		{
			Removed.Add(LastGEHandle);
			if (UGASDebuggerSettings::Get()->bEnableLogging)
			{
				const UGameplayEffect* GE = GetGameplayEffectFromSpec(GetGameplayEffectSpecFromHandle(OwningAbilitySystemComponent, LastGEHandle));
				FString GEName = GE ? GE->GetName() : TEXT("Unknown GE");
				FString LogMessage = FString::Printf(TEXT("%s - GE Removed: %s"), *FDateTime::Now().ToString(), *GEName);
				FGASDebuggerLogger::Log(ELogCategory::GameplayEffects, LogMessage);
			}
		}
	}

	// Find newly applied effects
	for (const FActiveGameplayEffectHandle& CurrentGEHandle : Current)
	{
		if (!LastActiveGameplayEffects.Contains(CurrentGEHandle))
		{
			Applied.Add(CurrentGEHandle);
			if (UGASDebuggerSettings::Get()->bEnableLogging)
			{
				const UGameplayEffect* GE = GetGameplayEffectFromSpec(GetGameplayEffectSpecFromHandle(OwningAbilitySystemComponent, CurrentGEHandle));
				FString GEName = GE ? GE->GetName() : TEXT("Unknown GE");
				FString LogMessage = FString::Printf(TEXT("%s - GE Applied: %s"), *FDateTime::Now().ToString(), *GEName);
				FGASDebuggerLogger::Log(ELogCategory::GameplayEffects, LogMessage);
			}
		}
	}
}
