// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_AttributeSlot.h"

#include "AbilitySystemComponent.h"

void UGASDebuggerUI_AttributeSlot::NativeDestruct()
{
	if (OwningAbilitySystemComponent)
	{
	 		// 解绑属性变化事件	
		OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
	}
	Super::NativeDestruct();
	
}

void UGASDebuggerUI_AttributeSlot::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	Super::InitAbilityWidget_Implementation(AbilitySystemComponent);
	if (OwningAbilitySystemComponent)
	{
		// 绑定属性变化事件
		OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &ThisClass::OnAttributeChanged);
	}
}

void UGASDebuggerUI_AttributeSlot::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeValueChanged( Data.NewValue, Data.OldValue);
}
