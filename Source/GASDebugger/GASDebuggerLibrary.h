// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GASDebuggerLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GASDEBUGGER_API UGASDebuggerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	
	static FString GetTimeInfo();


};
