// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerPawn.h"

#include "AbilitySystemComponent.h"


// Sets default values
AGASDebuggerPawn::AGASDebuggerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DebuggerAttributeSet = CreateDefaultSubobject<UGASDebuggerAttributeSet>(TEXT("DebuggerAttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AGASDebuggerPawn::BeginPlay()
{
	Super::BeginPlay();
	
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

