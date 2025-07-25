// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerPawn.h"

#include "AbilitySystemComponent.h"


// Sets default values
AGASDebuggerPawn::AGASDebuggerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DebuggerAttributeSet = CreateDefaultSubobject<UGASDebuggerAttributeSet>(TEXT("DebuggerAttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UGASDebuggerAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AGASDebuggerPawn::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	for (auto&& GA: AbilitiesToGrant)
	{
		if (GA == nullptr)
		{
			continue;
		}
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA, 1, INDEX_NONE, this));
		UE_LOG(LogTemp, Log, TEXT("GASDebuggerPawn: Granted ability %s."), *GA->GetName());
	}
}

// Called every frame
void AGASDebuggerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGASDebuggerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

