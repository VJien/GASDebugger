// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GASDebuggerWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "GASDebuggerUI_Tags.generated.h"

class UGridPanel;
class UVerticalBox;


USTRUCT(BlueprintType)
struct FGASDebuggerTagInfo
{
	GENERATED_BODY()
	FGASDebuggerTagInfo ()
	{
		Tag = FGameplayTag::EmptyTag;
		Info = FString();
	}
	FGASDebuggerTagInfo(const FGameplayTag& InTag, const FString& InInfo)
		: Tag(InTag), Info(InInfo)
	{
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Info;
};


UCLASS()
class GASDEBUGGER_API UGASDebuggerUI_Tags : public UGASDebuggerWidgetBase
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	virtual void InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent) override;
	void UpdateTags();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void RefreshRemovedTagsWidget(const TArray<FGASDebuggerTagInfo>& RemovedTags);

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void RefreshCurrentTagsWidget(const TArray<FGameplayTag>& CurrentTags);
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UVerticalBox* TagsBox_Current = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UGridPanel* TagsBox_Removed = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	int32 MaxTagAmount = 20;
	
	UPROPERTY()
	FGameplayTagContainer TagsContainer_Current;
	UPROPERTY()
	FGameplayTagContainer TagsContainer_Old;
	UPROPERTY()
	TArray<FGASDebuggerTagInfo> Tags_Old;

	

};
