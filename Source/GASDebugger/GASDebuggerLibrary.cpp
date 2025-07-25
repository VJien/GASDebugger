// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerLibrary.h"

int32 UGASDebuggerLibrary::GetAbilityActiveCount(UAbilitySystemComponent* AbilitySystemComponent,
	TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (AbilitySystemComponent == nullptr || AbilityClass == nullptr)
	{
		return -1;
	}
	auto AbilitySpecs =  AbilitySystemComponent->GetActivatableAbilities();
	for (auto&& Spec: AbilitySpecs)
	{
		if (Spec.Ability->GetClass() == AbilityClass)
		{
			return Spec.ActiveCount;
		}
	}
	return 0;
}
