// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MGCharacter.generated.h"

UCLASS()
class MGD_TEMPLATE_API AMGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMGCharacter();
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void PMoveCharacter(FVector2D Axis);

	UFUNCTION(BlueprintPure, Category="Shooting")
	bool IsHoldingTrigger() const { return bIsHoldingTrigger; }

	UFUNCTION(BlueprintCallable, Category="Shooting")
	void HoldTrigger(bool pullOrRelease);

	UFUNCTION(BlueprintImplementableEvent, Category="Shooting")
	void OnTriggerChanged(bool pullOrRelease);

	UFUNCTION(Server, Reliable, Category="Shooting")
	void Server_OnTriggerChanged(const bool pullOrRelease);

	UFUNCTION(NetMulticast, Reliable, Category="Shooting")
	void Multi_OnTriggerChanged(const bool pullOrRelease);

	
private:
	// returns if the player is holding down the trigger
	bool bIsHoldingTrigger;
	
	
};
