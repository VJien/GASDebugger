// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerUI_Tags.h"

#include "AbilitySystemComponent.h"
#include "Components/VerticalBox.h"

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
	OwningAbilitySystemComponent->GetOwnedGameplayTags(TagsContainer_Current);
	if (TagsContainer_Current != TagsContainer_Old)
	{
		//新的tags
		TArray<FGameplayTag> NewTags;
		TagsContainer_Current.GetGameplayTagArray(NewTags);
		RefreshCurrentTagsWidget(NewTags);
		//刷新之前的tags
		TagsContainer_Old.GetGameplayTagArray(Tags_Old);
		//合并新的旧的tags
		for (auto&& tag:NewTags)
		{
			Tags_Old.AddUnique(tag);
		}
		//移除所有newtags，得到removedtags
		Tags_Old.RemoveAll([&NewTags](const FGameplayTag& Tag)
		{
			return NewTags.Contains(Tag);
		});
		if (Tags_Old.Num()>0)
		{
			RefreshRemovedTagsWidget(Tags_Old);
		}
	}
	TagsContainer_Old = TagsContainer_Current;
}


