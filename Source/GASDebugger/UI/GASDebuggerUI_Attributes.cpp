// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Attributes.h"
#include "AbilitySystemComponent.h"
#include "GASDebugger/GASDebuggerSettings.h"
#include "GASDebugger/Demo/GASDebuggerLogger.h"

void UGASDebuggerUI_Attributes::InitAbilityWidget_Implementation(UAbilitySystemComponent* InOwningAbilitySystemComponent)
{
	Super::InitAbilityWidget_Implementation(InOwningAbilitySystemComponent);

	if (OwningAbilitySystemComponent)
	{
		TArray<FGameplayAttribute> Attributes;
		OwningAbilitySystemComponent->GetAllAttributes(Attributes);

		for (const FGameplayAttribute& Attribute : Attributes)
		{
			OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UGASDebuggerUI_Attributes::OnAttributeChanged);
		}
	}
}

void UGASDebuggerUI_Attributes::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (UGASDebuggerSettings::Get()->bEnableLogging)
	{
		FString LogMessage = FString::Printf(TEXT("%s - Attribute Changed: %s from %f to %f"), *FDateTime::Now().ToString(), *Data.Attribute.GetName(), Data.OldValue, Data.NewValue);
		FGASDebuggerLogger::Log(ELogCategory::Attributes, LogMessage);
	}
}

void UGASDebuggerUI_Attributes::NativeConstruct()
{
	Super::NativeConstruct();
}

TArray<FGameplayAttribute> UGASDebuggerUI_Attributes::GetAttributes() const
{
	TArray<FGameplayAttribute> Attributes;
	if (OwningAbilitySystemComponent)
	{
		OwningAbilitySystemComponent->GetAllAttributes(Attributes);
	}
	return Attributes;
}
