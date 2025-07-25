// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GASDebuggerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GASDEBUGGER_API UGASDebuggerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "")
	FGameplayAttributeData Test_Health;

	UPROPERTY(BlueprintReadOnly, Category = "")
	FGameplayAttributeData Test_Mana;
};
