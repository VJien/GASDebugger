// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerWidgetBase.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UGASDebuggerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGASDebuggerWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();
}



UWorld* UGASDebuggerWidgetBase::GetEditorWorld() const
{
	if (GEditor)
	{
		return GEditor->GetEditorWorldContext().World();
	}
	return nullptr;
}

UWorld* UGASDebuggerWidgetBase::GetPIEWorld() const
{
	if (GEditor)
	{
		if (GEditor->GetPIEWorldContext())
		{
			return GEditor->GetPIEWorldContext()->World();
		}
	}
	return nullptr;
}

bool UGASDebuggerWidgetBase::RunDirectly()
{
	bool bInit = false;
	if (auto&& OwningPlayer = GetOwningPlayer())
	{
		if (auto&& ASC = OwningPlayer->FindComponentByClass<UAbilitySystemComponent>())
		{
			InitAbilityWidget(ASC);
			bInit = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No AbilitySystemComponent found on OwningPlayer."));
		}
		if (bInit)
		{
			return true;
		}
		
		if (auto&& PC = Cast<ACharacter>(OwningPlayer->GetPawn()))
		{
			auto&& ASC = PC->FindComponentByClass<UAbilitySystemComponent>();
			if (ASC)
			{
				InitAbilityWidget(ASC);
				bInit = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No AbilitySystemComponent found on PlayerCharacter."));
			}
		}
	}
	else 
	{
		if (auto&& World = GetPIEWorld())
		{
			
			if (auto&& PlayerPawn = UGameplayStatics::GetPlayerPawn(World,0))
			{
				auto&& ASC = PlayerPawn->FindComponentByClass<UAbilitySystemComponent>();
				if (ASC)
				{
					InitAbilityWidget(ASC);
					bInit = true;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No AbilitySystemComponent found on PlayerPawn."));
				}
			}
			if (bInit)
			{
				return true;
			}
			if (auto&& PC = Cast<ACharacter>(UGameplayStatics::GetPlayerController(World, 0)))
			{
				auto&& ASC = PC->FindComponentByClass<UAbilitySystemComponent>();
				if (ASC)
				{
					InitAbilityWidget(ASC);
					bInit = true;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No AbilitySystemComponent found on PlayerCharacter."));
				}
			}
		}
	}
	return bInit;
}

void UGASDebuggerWidgetBase::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	OwningAbilitySystemComponent = AbilitySystemComponent;
}
