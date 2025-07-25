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
	virtual FName GetCategoryName() const override { return TEXT("Game"); }

	virtual FName GetSectionName() const override { return TEXT("GAS Debugger "); }
public:
	static UGASDebuggerSettings* Get()
	{
		return GetMutableDefault<UGASDebuggerSettings>();
	}

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Config)
	TSoftClassPtr<UUserWidget> Widget = nullptr;
};
