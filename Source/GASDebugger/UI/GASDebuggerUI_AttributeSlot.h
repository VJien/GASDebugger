// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "EditorUtilityWidget.h"
#include "GASDebuggerWidgetBase.h"
#include "GASDebuggerUI_AttributeSlot.generated.h"



UCLASS()
class GASDEBUGGER_API UGASDebuggerUI_AttributeSlot : public UGASDebuggerWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;
protected:
	virtual void InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)override;
	 
	UPROPERTY(BlueprintReadOnly,EditAnywhere, meta=(ExposeOnSpawn = true))
	FGameplayAttribute Attribute;

	void OnAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttributeValueChanged(float NewValue, float OldValue);
};
