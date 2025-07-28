// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASDebuggerWidgetBase.h"
#include "GASDebuggerUI_Abilities.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FGASDebuggerUI_AbilitiesData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UGameplayAbility* Ability = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString Info;
};



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
	void RefreshRemovedAbilitiesWidget(const TArray<FGASDebuggerUI_AbilitiesData>& _RemovedAbilities);
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void RefreshCurrentAbilitiesWidget(const TArray<UGameplayAbility*>& _CurrentAbilities);

protected:
	
	UPROPERTY(BlueprintReadOnly)
	TArray<class UGameplayAbility*> CurrentAbilities;
	UPROPERTY(BlueprintReadOnly)
	TArray<UGameplayAbility*> RemovedAbilities;
};
