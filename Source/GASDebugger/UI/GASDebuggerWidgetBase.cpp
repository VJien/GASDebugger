// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerWidgetBase.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UGASDebuggerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	FEditorDelegates::BeginPIE.AddUObject(this, &ThisClass::OnPIEStarted);
	FEditorDelegates::EndPIE.AddUObject(this, &ThisClass::OnPIEEnded);

}

void UGASDebuggerWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UGASDebuggerWidgetBase::NativeDestruct()
{
	FEditorDelegates::BeginPIE.RemoveAll(this);
	FEditorDelegates::EndPIE.RemoveAll(this);
	Super::NativeDestruct();
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

void UGASDebuggerWidgetBase::OnPIEStarted(bool bIsSimulating)
{
	UE_LOG(LogTemp, Log, TEXT("GASDebuggerWidgetBase: PIE started. Simulating: %s"), bIsSimulating ? TEXT("true") : TEXT("false"));
	OnPIEStart();
}

void UGASDebuggerWidgetBase::OnPIEEnded(bool bIsSimulating)
{
	UE_LOG(LogTemp, Log, TEXT("GASDebuggerWidgetBase: PIE ended. Simulating: %s"), bIsSimulating ? TEXT("true") : TEXT("false"));
	OnPIEEnd();
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
