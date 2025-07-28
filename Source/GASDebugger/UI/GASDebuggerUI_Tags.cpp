// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Tags.h"

#include "AbilitySystemComponent.h"
#include "Components/VerticalBox.h"
#include "GASDebugger/GASDebuggerLibrary.h"
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
	UpdateTags();
}

void UGASDebuggerUI_Tags::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	OwningAbilitySystemComponent = AbilitySystemComponent;
	Tags_Old.Empty();
	TagsContainer_Current = FGameplayTagContainer();
	TagsContainer_Old = FGameplayTagContainer();
}

void UGASDebuggerUI_Tags::UpdateTags()
{
	if (OwningAbilitySystemComponent ==nullptr)
	{
		return;
	}
	TagsContainer_Current.Reset();
	OwningAbilitySystemComponent->GetOwnedGameplayTags(TagsContainer_Current);
	//新的tags
	TArray<FGameplayTag> NewTags;
	TagsContainer_Current.GetGameplayTagArray(NewTags);
	RefreshCurrentTagsWidget(NewTags);
	if (TagsContainer_Current != TagsContainer_Old)
	{
		
		//刷新之前的tags
		TArray<FGameplayTag> Old;
		TagsContainer_Old.GetGameplayTagArray(Old);
		Tags_Old.Empty();
		//合并新的旧的tags
		for (auto&& tag:NewTags)
		{
			Old.AddUnique(tag);
		}
		//移除所有newtags，得到removedtags
		Old.RemoveAll([&NewTags](const FGameplayTag& Tag)
		{
			return NewTags.Contains(Tag);
		});
		if (Old.Num()>0)
		{
			for (auto&& tag:Old)
			{
				Tags_Old.Add({tag, UGASDebuggerLibrary::GetTimeInfo()});
			}
			RefreshRemovedTagsWidget(Tags_Old);
		}
	}
	TagsContainer_Old = TagsContainer_Current;
}


