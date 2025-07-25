// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASDebuggerWidgetBase.h"
#include "GASDebuggerUI_Abilities.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class GASDEBUGGER_API UGASDebuggerUI_Abilities : public UGASDebuggerWidgetBase
{
	GENERATED_BODY()
public:
	virtual void InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void UpdateAbilities();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void RefreshRemovedAbilitiesWidget(const TArray<UGameplayAbility*>& _RemovedAbilities);
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void RefreshCurrentAbilitiesWidget(const TArray<UGameplayAbility*>& _CurrentAbilities);

protected:
	
	UPROPERTY(BlueprintReadOnly)
	TArray<class UGameplayAbility*> CurrentAbilities;
	UPROPERTY(BlueprintReadOnly)
	TArray<UGameplayAbility*> RemovedAbilities;
};
