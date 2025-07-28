// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_GE.h"



#include "AbilitySystemComponent.h"

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
#if UE5_OR_LATER
	const FActiveGameplayEffectsContainer& GeContainer = OwningAbilitySystemComponent->GetActiveGameplayEffects();
	CurrActiveGameplayEffects = GeContainer.GetAllActiveEffectHandles();
#else
	CurrActiveGameplayEffects = OwningAbilitySystemComponent->GetActiveEffects(FGameplayEffectQuery());
#endif
	
	
	

	if (CurrActiveGameplayEffects.Num() == 0 && LastActiveGameplayEffects.Num() == 0)
	{
		return;
	}
	
	
	// 初始化比较
	CompareGEs(CurrActiveGameplayEffects, RemovedGameplayEffects, AppliedGameplayEffects);
	
	// 更新上次活动效果
	LastActiveGameplayEffects = CurrActiveGameplayEffects;
	if (AppliedGameplayEffects.Num()>0)
	{
		OnGameplayEffectApplied(AppliedGameplayEffects);
	}
	if (RemovedGameplayEffects.Num()>0)
	{
		OnGameplayEffectRemoved(RemovedGameplayEffects);
	}
}

void UGASDebuggerUI_GE::CompareGEs(const TArray<FActiveGameplayEffectHandle>& Current,  TArray<FActiveGameplayEffectHandle>& Removed, TArray<FActiveGameplayEffectHandle>& Applied)
{
	//比较Current和LastActiveGameplayEffects，找出Removed和Applied

	// 清空 Removed 和 Applied 列表
	Removed.Empty();
	Applied.Empty();
	// 找出已移除的效果
	for (auto&& LastGE : LastActiveGameplayEffects)
	{
		if (!Current.Contains(LastGE))
		{
			Removed.Add(LastGE);
		}
	}
	// 找出新增的效果
	for (auto&& CurrentGE : Current)
	{
		if (!LastActiveGameplayEffects.Contains(CurrentGE))
		{
			Applied.Add(CurrentGE);
		}
	}

}

