﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GASDebuggerWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "GASDebuggerUI_GE.generated.h"

/**
 * 
 */
UCLASS()
class GASDEBUGGER_API UGASDebuggerUI_GE : public UGASDebuggerWidgetBase
{
	GENERATED_BODY()
public:
	virtual void InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;



protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameplayEffectRemoved(const TArray<FActiveGameplayEffectHandle>& RemovedGameplayEffect);
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameplayEffectApplied(const TArray<FActiveGameplayEffectHandle>& AppliedGameplayEffect);

	
	void CompareGEs(const TArray<FActiveGameplayEffectHandle>& Current, TArray<FActiveGameplayEffectHandle>& Removed, TArray<FActiveGameplayEffectHandle>& Applied);

protected:
	// Cache for active GE info. Key is the handle, Value is the GE's name.
	// This allows us to retrieve the name of a GE even after it has been removed.
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, FString> ActiveGEInfos;

	// Variables for UI updates
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> CurrActiveGameplayEffects;
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> RemovedGameplayEffects;
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> AppliedGameplayEffects;
};
