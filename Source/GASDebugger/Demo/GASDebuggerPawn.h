﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASDebuggerAbilitySystemComponent.h"
#include "GASDebuggerAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GASDebuggerPawn.generated.h"

class UAbilitySystemComponent;
UCLASS()
class GASDEBUGGER_API AGASDebuggerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGASDebuggerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




protected:
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UGASDebuggerAttributeSet* DebuggerAttributeSet = nullptr;
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
 	UGASDebuggerAbilitySystemComponent* AbilitySystemComponent = nullptr;


	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;
	
};
