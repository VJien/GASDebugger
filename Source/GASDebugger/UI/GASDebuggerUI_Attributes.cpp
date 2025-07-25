// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Attributes.h"
#include "AbilitySystemComponent.h"

void UGASDebuggerUI_Attributes::InitAbilityWidget_Implementation(UAbilitySystemComponent* InOwningAbilitySystemComponent)
{
	OwningAbilitySystemComponent = InOwningAbilitySystemComponent;
}

void UGASDebuggerUI_Attributes::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGASDebuggerUI_Attributes::NativeConstruct()
{
	Super::NativeConstruct();
}

TArray<FGameplayAttribute> UGASDebuggerUI_Attributes::GetAttributes() const
{
	TArray<FGameplayAttribute> Attributes;
	OwningAbilitySystemComponent->GetAllAttributes(Attributes);
	return Attributes;
}
