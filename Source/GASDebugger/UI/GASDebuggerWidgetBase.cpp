// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerWidgetBase.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GASDebugger/Demo/GASDebuggerLogger.h"
#include "Kismet/GameplayStatics.h"

void UGASDebuggerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	FEditorDelegates::PostPIEStarted.AddUObject(this, &ThisClass::OnPIEStarted);
	FEditorDelegates::EndPIE.AddUObject(this, &ThisClass::OnPIEEnded);
	auto&& Wd = GetPIEWorld();
	
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

void UGASDebuggerWidgetBase::OnActorsInitialized(const UWorld::FActorsInitializedParams& Params)
{
	OnPIEStart();
}

bool UGASDebuggerWidgetBase::RunDirectly(AActor*& OutASCOwner, UAbilitySystemComponent*& OutASC)
{
	bool bInit = false;
	if (auto&& OwningPlayer = GetOwningPlayer())
	{
		if (auto&& ASC = OwningPlayer->FindComponentByClass<UAbilitySystemComponent>())
		{
			InitAbilityWidget(ASC);
			OutASC = ASC;
			OutASCOwner = OwningPlayer;
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
				OutASCOwner = PC;
				OutASC = ASC;
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
					OutASCOwner = PlayerPawn;
					OutASC = ASC;
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
					OutASCOwner = PC;
					OutASC = ASC;
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

void UGASDebuggerWidgetBase::OpenLogDirectory()
{
	if (GEditor)
	{
		FString LogDirectory = FGASDebuggerLogger::GetLogFilePath();
		//相对路径转换成绝对路径
		if (!FPaths::IsRelative(LogDirectory))
		{
			LogDirectory = FPaths::ConvertRelativePathToFull(LogDirectory);
		}
		FWindowsPlatformProcess::ExploreFolder(*LogDirectory);
		UE_LOG(LogTemp, Log, TEXT("Opened log directory: %s"), *LogDirectory);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GEditor is null, cannot open log directory."));
	}
}

FGameplayEffectSpec UGASDebuggerWidgetBase::GetGameplayEffectSpecFromHandle(
	UAbilitySystemComponent* AbilitySystemComponent, FActiveGameplayEffectHandle Handle)
{
	if (AbilitySystemComponent && Handle.IsValid())
	{
		auto GE = AbilitySystemComponent->GetActiveGameplayEffect(Handle);
		if (GE)
		{
			return GE->Spec;
		}
	}
	return FGameplayEffectSpec();
}

float UGASDebuggerWidgetBase::GetGameplayEffectDefaultDuration(FGameplayEffectSpec Spec)
{
	return Spec.Duration;
}

float UGASDebuggerWidgetBase::GetGameplayEffectCurrentTimeByHandle(UAbilitySystemComponent* AbilitySystemComponent,
	FActiveGameplayEffectHandle Handle)
{
	auto GE =  AbilitySystemComponent->GetActiveGameplayEffect(Handle);
	return GE->GetTimeRemaining(AbilitySystemComponent->GetWorld()->GetTimeSeconds());
}

TArray<FGameplayEffectSpec> UGASDebuggerWidgetBase::GetAllActiveGameplayEffects(
	UAbilitySystemComponent* AbilitySystemComponent)
{
	TArray<FGameplayEffectSpec> Results;
	if (AbilitySystemComponent == nullptr)
	{
		return Results;
	}
	AbilitySystemComponent->GetAllActiveGameplayEffectSpecs(Results);
	return Results;
}

const UGameplayEffect* UGASDebuggerWidgetBase::GetGameplayEffectFromSpec(FGameplayEffectSpec Spec)
{
	return Spec.Def;
}
int32 UGASDebuggerWidgetBase::GetAbilityActiveCount(UAbilitySystemComponent* AbilitySystemComponent,
												 TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (AbilitySystemComponent == nullptr || AbilityClass == nullptr)
	{
		return -1;
	}
	auto AbilitySpecs =  AbilitySystemComponent->GetActivatableAbilities();
	for (auto&& Spec: AbilitySpecs)
	{
		if (Spec.Ability->GetClass() == AbilityClass)
		{
			return Spec.ActiveCount;
		}
	}
	return 0;
}

int32 UGASDebuggerWidgetBase::GetGameplayTagCount(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (AbilitySystemComponent == nullptr || !Tag.IsValid())
	{
		return -1;
	}
	return AbilitySystemComponent->GetTagCount(Tag);
}
FString UGASDebuggerWidgetBase::GetAttributeName(FGameplayAttribute Attribute)
{
	return Attribute.GetName();
}

bool UGASDebuggerWidgetBase::EqualEqual_ActiveGameplayEffectHandle(const FActiveGameplayEffectHandle& A,
	const FActiveGameplayEffectHandle& B)
{
	return  A == B;
}

void UGASDebuggerWidgetBase::InitAbilityWidget_Implementation(UAbilitySystemComponent* AbilitySystemComponent)
{
	OwningAbilitySystemComponent = AbilitySystemComponent;
}
