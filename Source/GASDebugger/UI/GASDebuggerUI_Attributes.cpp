// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Attributes.h"
#include "AbilitySystemComponent.h"
#include "GASDebugger/GASDebuggerSettings.h"
#include "GASDebugger/Demo/GASDebuggerLogger.h"

void UGASDebuggerUI_Attributes::InitAbilityWidget_Implementation(UAbilitySystemComponent* InOwningAbilitySystemComponent)
{
	Super::InitAbilityWidget_Implementation(InOwningAbilitySystemComponent);
	TimeSinceLastLog = 0.0f;
	PendingAttributeChanges.Empty();

	if (OwningAbilitySystemComponent)
	{
		TArray<FGameplayAttribute> Attributes;
		OwningAbilitySystemComponent->GetAllAttributes(Attributes);

		for (const FGameplayAttribute& Attribute : Attributes)
		{
			OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this); // 清除之前的委托绑定
			OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UGASDebuggerUI_Attributes::OnAttributeChanged);
		}
	}
}

void UGASDebuggerUI_Attributes::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const UGASDebuggerSettings* Settings = UGASDebuggerSettings::Get();
	if (!Settings || !Settings->bEnableLogging || PendingAttributeChanges.Num() == 0)
	{
		return;
	}

	TimeSinceLastLog += InDeltaTime;

	if (TimeSinceLastLog >= Settings->AttributeLogUpdateFrequency)
	{
		for (auto&&  Pair : PendingAttributeChanges)
		{
			auto&& Attribute = Pair.Key;
			auto&& ChangeInfo = Pair.Value;
			
			FString LogMessage = FString::Printf(
				TEXT("%s - Attribute Changed: %s from %.*f to %.*f (x%d changes)"),
				*FDateTime::Now().ToString(),
				*Attribute.GetName(),
				Settings->AttributeLogDecimalPrecision, // %.*f 的第一个参数：精度
				ChangeInfo.InitialValue,             // %.*f 的第二个参数：浮点数值
				Settings->AttributeLogDecimalPrecision, // %.*f 的第一个参数：精度
				ChangeInfo.LatestValue,              // %.*f 的第二个参数：浮点数值
				ChangeInfo.ChangeCount
			);

			FGASDebuggerLogger::Log(ELogCategory::Attributes, LogMessage);
		}

		PendingAttributeChanges.Empty();
		TimeSinceLastLog = 0.0f;
	}
}

void UGASDebuggerUI_Attributes::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (!UGASDebuggerSettings::Get()->bEnableLogging)
	{
		return;
	}
	if (FMath::IsNearlyEqual(Data.OldValue, Data.NewValue))
	{
		return;
	}
	// If update frequency is 0, log immediately
	if (UGASDebuggerSettings::Get()->AttributeLogUpdateFrequency == 0.f)
	{
		const int32 Precision = UGASDebuggerSettings::Get()->AttributeLogDecimalPrecision;
		
		FString LogMessage = FString::Printf(
			TEXT("%s - Attribute Changed: %s from %.*f to %.*f"),
			*FDateTime::Now().ToString(),
			*Data.Attribute.GetName(),
			Precision,      // %.*f 的第一个参数：精度
			Data.OldValue,  // %.*f 的第二个参数：浮点数值
			Precision,      // %.*f 的第一个参数：精度
			Data.NewValue   // %.*f 的第二个参数：浮点数值
		);

		FGASDebuggerLogger::Log(ELogCategory::Attributes, LogMessage);
		return;
	}

	// Otherwise, aggregate the changes
	FAttributeChangeInfo& Info = PendingAttributeChanges.FindOrAdd(Data.Attribute);
	if (Info.ChangeCount == 0)
	{
		Info.InitialValue = Data.OldValue;
	}
	Info.LatestValue = Data.NewValue;
	Info.ChangeCount++;
}

void UGASDebuggerUI_Attributes::NativeConstruct()
{
	if (OwningAbilitySystemComponent)
	{
		TArray<FGameplayAttribute> Attributes;
		OwningAbilitySystemComponent->GetAllAttributes(Attributes);
		for (const FGameplayAttribute& Attribute : Attributes)
		{
			OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this); // 清除之前的委托绑定
		}
	}
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
