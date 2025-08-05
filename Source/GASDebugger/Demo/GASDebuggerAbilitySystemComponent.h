// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASDebuggerAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Nova), meta=(BlueprintSpawnableComponent),BlueprintType,Blueprintable)
class GASDEBUGGER_API UGASDebuggerAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGASDebuggerAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = "ASC")
	FGameplayAbilitySpecHandle AddNewAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 level);

	UFUNCTION(BlueprintCallable, Category = "ASC")
	bool RemoveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass, FGameplayAbilitySpecHandle& OutHandle);



	UFUNCTION(BlueprintCallable, Category = "Ability")
	void AddTag(FGameplayTag Tag,int32 Count = 1);
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void AddTags(FGameplayTagContainer TagContainer);
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveTag(FGameplayTag Tag,int32 Count = 1);
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveTags(FGameplayTagContainer TagContainer);
};

