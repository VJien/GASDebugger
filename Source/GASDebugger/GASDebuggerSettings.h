// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GASDebuggerSettings.generated.h"

class UUserWidget;

UCLASS(BlueprintType, Config=Nova, defaultconfig, DisplayName = "GAS Debugger")
class GASDEBUGGER_API UGASDebuggerSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	UGASDebuggerSettings();
	
	virtual FName GetCategoryName() const override { return TEXT("Game"); }

	virtual FName GetSectionName() const override { return TEXT("GAS Debugger "); }
public:
	static UGASDebuggerSettings* Get()
	{
		return GetMutableDefault<UGASDebuggerSettings>();
	}

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Config, meta=(FilePathFilter = "GASDebugger", DisplayName = "Debugger Widget",AllowedClasses = "EditorUtilityWidget"))
	TSoftClassPtr<UUserWidget> Widget = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, meta = (DisplayName = "Enable Logging"))
	bool bEnableLogging = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, meta = (DisplayName = "Attribute Log Update Frequency (s)", ClampMin = "0.0", EditCondition = "bEnableLogging",HideEditCondition))
	float AttributeLogUpdateFrequency = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, meta = (DisplayName = "Attribute Log Decimal Precision", ClampMin = "0", ClampMax = "6", EditCondition = "bEnableLogging",HideEditCondition))
	int32 AttributeLogDecimalPrecision = 2;
};
