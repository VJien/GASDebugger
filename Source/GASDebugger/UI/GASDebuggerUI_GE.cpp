// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_GE.h"
#include "AbilitySystemComponent.h"
#include "GASDebugger/GASDebuggerSettings.h"
#include "GASDebugger/Demo/GASDebuggerLogger.h"

void UGASDebuggerUI_GE::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	Super::InitAbilityWidget_Implementation(AbilitySystemComponent);
	ActiveGEInfos.Empty();
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
	FGameplayEffectQuery Query;
	Query.EffectSource = nullptr;
	CurrActiveGameplayEffects = OwningAbilitySystemComponent->GetActiveEffects(Query);
#endif
	
	if (CurrActiveGameplayEffects.Num() == 0 && ActiveGEInfos.Num() == 0)
	{
		return;
	}
	
	// Compare and find differences
	CompareGEs(CurrActiveGameplayEffects, RemovedGameplayEffects, AppliedGameplayEffects);
	
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

	TArray<FActiveGameplayEffectHandle> LastHandles;
	ActiveGEInfos.GetKeys(LastHandles);

	// Find removed effects
	for (const FActiveGameplayEffectHandle& LastGEHandle : LastHandles)
	{
		if (!Current.Contains(LastGEHandle))
		{
			Removed.Add(LastGEHandle);
			if (UGASDebuggerSettings::Get()->bEnableLogging)
			{
				// Get the name from our cache
				FString GEName = ActiveGEInfos.FindRef(LastGEHandle);
				FString LogMessage = FString::Printf(TEXT("%s - GE Removed: %s"), *FDateTime::Now().ToString(), *GEName);
				FGASDebuggerLogger::Log(ELogCategory::GameplayEffects, LogMessage);
			}
			// Remove from the cache
			ActiveGEInfos.Remove(LastGEHandle);
		}
	}

	// Find newly applied effects
	for (const FActiveGameplayEffectHandle& CurrentGEHandle : Current)
	{
		if (!ActiveGEInfos.Contains(CurrentGEHandle))
		{
			Applied.Add(CurrentGEHandle);
			FString GEName = TEXT("Unknown GE");
			if (UGASDebuggerSettings::Get()->bEnableLogging)
			{
				// Get the name while the handle is still valid
				const UGameplayEffect* GE = GetGameplayEffectFromSpec(GetGameplayEffectSpecFromHandle(OwningAbilitySystemComponent, CurrentGEHandle));
				if (GE)
				{
					GEName = GE->GetName();
				}
				FString LogMessage = FString::Printf(TEXT("%s - GE Applied: %s"), *FDateTime::Now().ToString(), *GEName);
				FGASDebuggerLogger::Log(ELogCategory::GameplayEffects, LogMessage);
			}
			// Add to our cache for later
			ActiveGEInfos.Add(CurrentGEHandle, GEName);
		}
	}
}
