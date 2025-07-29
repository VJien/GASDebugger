// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Tags.h"

#include "AbilitySystemComponent.h"
#include "Components/VerticalBox.h"
#include "GASDebugger/GASDebuggerLibrary.h"
#include "GASDebugger/GASDebuggerSettings.h"
#include "GASDebugger/Demo/GASDebuggerLogger.h"
#include "Kismet/KismetMathLibrary.h"

void UGASDebuggerUI_Tags::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UGASDebuggerUI_Tags::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGASDebuggerUI_Tags::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGASDebuggerUI_Tags::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// UI update logic (original method)
	UpdateTags();

	// Independent logging logic for count changes
	LogTagCountChanges();
}

void UGASDebuggerUI_Tags::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	Super::InitAbilityWidget_Implementation(AbilitySystemComponent);
	
	// Initialize state for UI polling
	Tags_Old.Empty();
	TagsContainer_Current = FGameplayTagContainer();
	TagsContainer_Old = FGameplayTagContainer();

	// Initialize state for logging
	OldTagCounts.Empty();
	if (OwningAbilitySystemComponent)
	{
		FGameplayTagContainer InitialTags;
		OwningAbilitySystemComponent->GetOwnedGameplayTags(InitialTags);
		for (const FGameplayTag& Tag : InitialTags)
		{
			OldTagCounts.Add(Tag, OwningAbilitySystemComponent->GetTagCount(Tag));
		}
	}
}

void UGASDebuggerUI_Tags::LogTagCountChanges()
{
	if (OwningAbilitySystemComponent == nullptr || !UGASDebuggerSettings::Get()->bEnableLogging)
	{
		return;
	}

	TMap<FGameplayTag, int32> NewTagCounts;
	FGameplayTagContainer AllTags;
	OwningAbilitySystemComponent->GetOwnedGameplayTags(AllTags);

	for (const FGameplayTag& Tag : AllTags)
	{
		NewTagCounts.Add(Tag, OwningAbilitySystemComponent->GetTagCount(Tag));
	}

	// Check for changed or added tags
	for (const auto& Pair : NewTagCounts)
	{
		const FGameplayTag& Tag = Pair.Key;
		const int32 NewCount = Pair.Value;
		const int32 OldCount = OldTagCounts.FindRef(Tag); // Returns 0 if not found

		if (NewCount != OldCount)
		{
			FString Action = (NewCount > OldCount) ? TEXT("Added or Increased") : TEXT("Decreased");
			FString LogMessage = FString::Printf(TEXT("%s - Tag %s: %s (Old: %d, New: %d)"), *FDateTime::Now().ToString(), *Tag.ToString(), *Action, OldCount, NewCount);
			FGASDebuggerLogger::Log(ELogCategory::Tags, LogMessage);
		}
	}

	// Check for removed tags
	for (const auto& Pair : OldTagCounts)
	{
		const FGameplayTag& Tag = Pair.Key;
		if (!NewTagCounts.Contains(Tag))
		{
			FString LogMessage = FString::Printf(TEXT("%s - Tag Removed: %s (Old: %d, New: 0)"), *FDateTime::Now().ToString(), *Tag.ToString(), Pair.Value);
			FGASDebuggerLogger::Log(ELogCategory::Tags, LogMessage);
		}
	}

	// Update old counts for the next frame
	OldTagCounts = NewTagCounts;
}

void UGASDebuggerUI_Tags::UpdateTags()
{
	if (OwningAbilitySystemComponent == nullptr)
	{
		return;
	}
	TagsContainer_Current.Reset();
	// This is the original UI update logic, it remains unchanged.
	OwningAbilitySystemComponent->GetOwnedGameplayTags(TagsContainer_Current);
	TArray<FGameplayTag> NewTags;
	TagsContainer_Current.GetGameplayTagArray(NewTags);
	//New tags need update every frame, so we refresh the current tags widget.
	RefreshCurrentTagsWidget(NewTags);
	if (TagsContainer_Current != TagsContainer_Old)
	{
		TArray<FGameplayTag> OldTags;
		TagsContainer_Old.GetGameplayTagArray(OldTags);

		// This logic is only for the UI's "removed" list, not for logging.
		for (const FGameplayTag& OldTag : OldTags)
		{
			if (!TagsContainer_Current.HasTag(OldTag))
			{
				Tags_Old.Add({OldTag, UGASDebuggerLibrary::GetTimeInfo(), FDateTime::Now()});
			}
		}
		
		
		if(Tags_Old.Num() > 0)
		{
			RefreshRemovedTagsWidget(Tags_Old);
		}

		TagsContainer_Old = TagsContainer_Current;
	}
}

void UGASDebuggerUI_Tags::SortDebuggerTags(TArray<FGASDebuggerTagInfo>& Tags, bool bNewFirst)
{
	Tags.Sort([bNewFirst](const FGASDebuggerTagInfo& A, const FGASDebuggerTagInfo& B)
	{
		if (bNewFirst)
		{
			return A.LastUpdatedTime > B.LastUpdatedTime;
		}
		else
		{
			return A.LastUpdatedTime < B.LastUpdatedTime;
		}
	});
}

void UGASDebuggerUI_Tags::UniqueDebuggerTags(TArray<FGASDebuggerTagInfo>& Tags)
{
	TArray<FGASDebuggerTagInfo> UniqueTags;
	for (const FGASDebuggerTagInfo& TagInfo : Tags)
	{
		if (!UniqueTags.Contains(TagInfo))
		{
			UniqueTags.Add(TagInfo);
		}
	}
	Tags = UniqueTags;
}
