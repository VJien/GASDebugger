// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "GASDebuggerWidgetBase.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GASDEBUGGER_API UGASDebuggerWidgetBase : public UEditorUtilityWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	UWorld* GetEditorWorld()const;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	UWorld* GetPIEWorld()const;

	UFUNCTION()
	void OnPIEStarted(bool bIsSimulating);
	UFUNCTION()
	void OnPIEEnded(bool bIsSimulating);


	UFUNCTION(BlueprintImplementableEvent)
	void OnPIEStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPIEEnd();

	void OnActorsInitialized(const UWorld::FActorsInitializedParams& Params);
	
	//自动查询玩家类并运行
	UFUNCTION(BlueprintCallable)
	bool RunDirectly(AActor*& OutASCOwner, UAbilitySystemComponent*& OutASC);
	
	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* OwningAbilitySystemComponent = nullptr;




	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Nova|Ability", meta=(AutoCreateRefTerm = "Context"))
	FGameplayEffectSpec GetGameplayEffectSpecFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FActiveGameplayEffectHandle Handle);
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Nova|Ability", meta=(AutoCreateRefTerm = "Context"))
	float GetGameplayEffectDefaultDuration(FGameplayEffectSpec Spec);
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Nova|Ability", meta=(AutoCreateRefTerm = "Context"))
	float GetGameplayEffectCurrentTimeByHandle(UAbilitySystemComponent* AbilitySystemComponent, FActiveGameplayEffectHandle Handle);

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Nova|Ability", meta=(AutoCreateRefTerm = "Context"))
	TArray<FGameplayEffectSpec> GetAllActiveGameplayEffects(UAbilitySystemComponent* AbilitySystemComponent);
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Nova|Ability", meta=(AutoCreateRefTerm = "Context"))
	const UGameplayEffect* GetGameplayEffectFromSpec(FGameplayEffectSpec Spec);
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "", meta=(AutoCreateRefTerm = "Context"))
	int32 GetAbilityActiveCount(UAbilitySystemComponent* AbilitySystemComponent,  TSubclassOf<UGameplayAbility> AbilityClass);
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "", meta=(AutoCreateRefTerm = "Context"))
	int32 GetGameplayTagCount (UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "",meta=(AutoCreateRefTerm = "Context"))
	FString GetAttributeName(FGameplayAttribute Attribute);
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "AbilityWidget")
	void InitAbilityWidget(UAbilitySystemComponent* AbilitySystemComponent);
};

