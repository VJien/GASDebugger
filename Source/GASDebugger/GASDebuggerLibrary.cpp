// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerLibrary.h"

#include "Kismet/KismetMathLibrary.h"

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

int32 UGASDebuggerLibrary::GetGameplayTagCount(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (AbilitySystemComponent == nullptr || !Tag.IsValid())
	{
		return -1;
	}
	return AbilitySystemComponent->GetTagCount(Tag);
}

FString UGASDebuggerLibrary::GetTimeInfo()
{
	FString CurrentTime = FString::Printf(TEXT("%d:%d:%d:%d"), UKismetMathLibrary::Now().GetHour(),
				   UKismetMathLibrary::Now().GetMinute(),
				   UKismetMathLibrary::Now().GetSecond(),
				   UKismetMathLibrary::Now().GetMillisecond());
	return CurrentTime;
}
