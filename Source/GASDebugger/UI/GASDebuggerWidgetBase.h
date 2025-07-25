// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "GASDebuggerWidgetBase.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GASDEBUGGER_API UGASDebuggerWidgetBase : public UEditorUtilityWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	UWorld* GetEditorWorld()const;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	UWorld* GetPIEWorld()const;

	UFUNCTION()
	void OnPIEStarted(bool bIsSimulating);
	UFUNCTION()
	void OnPIEEnded(bool bIsSimulating);


	UFUNCTION(BlueprintImplementableEvent)
	void OnPIEStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPIEEnd();
	
	//自动查询玩家类并运行
	UFUNCTION(BlueprintCallable)
	bool RunDirectly();
	
	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* OwningAbilitySystemComponent = nullptr;

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "AbilityWidget")
	void InitAbilityWidget(UAbilitySystemComponent* AbilitySystemComponent);
};

