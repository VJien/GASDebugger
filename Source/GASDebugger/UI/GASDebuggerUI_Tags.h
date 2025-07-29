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
		LastUpdatedTime = FDateTime::MinValue();
	}
	FGASDebuggerTagInfo(const FGameplayTag& InTag, const FString& InInfo, const FDateTime& InLastUpdatedTime)
		: Tag(InTag), Info(InInfo), LastUpdatedTime(InLastUpdatedTime)
	{
	}
	bool operator==(const FGASDebuggerTagInfo& Other) const
	{
		return Tag == Other.Tag && Info == Other.Info && LastUpdatedTime == Other.LastUpdatedTime;
	}
	bool operator!=(const FGASDebuggerTagInfo& Other) const
	{
		return !(*this == Other);
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Info;

	FDateTime LastUpdatedTime; // Timestamp for when the tag was last updated
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
	
	UFUNCTION(BlueprintCallable)
	void SortDebuggerTags(UPARAM(ref)TArray<FGASDebuggerTagInfo>& Tags, bool bNewFirst = true);
	UFUNCTION(BlueprintCallable)
	void UniqueDebuggerTags(UPARAM(ref)TArray<FGASDebuggerTagInfo>& Tags);

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

protected:
	
	void LogTagCountChanges();

	// Map to store previous frame's tag counts, used for logging only
	TMap<FGameplayTag, int32> OldTagCounts;
};
