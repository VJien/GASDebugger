// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GASDebuggerWidgetBase.h"
#include "GASDebuggerUI_Attributes.generated.h"

struct FAttributeChangeInfo
{
	float InitialValue;
	float LatestValue;
	int32 ChangeCount;

	FAttributeChangeInfo() : InitialValue(0.f), LatestValue(0.f), ChangeCount(0) {}
};

class UNovaAttributeSet;
class UAbilitySystemComponent;
UCLASS()
class GASDEBUGGER_API UGASDebuggerUI_Attributes : public UGASDebuggerWidgetBase
{
	GENERATED_BODY()
public:
	virtual void InitAbilityWidget_Implementation(UAbilitySystemComponent* InOwningAbilitySystemComponent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
protected:
	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAttribute> GetAttributes() const;

	void OnAttributeChanged(const FOnAttributeChangeData& Data);

private:
	TMap<FGameplayAttribute, FAttributeChangeInfo> PendingAttributeChanges;
	float TimeSinceLastLog;
};
