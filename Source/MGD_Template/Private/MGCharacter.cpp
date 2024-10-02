// Fill out your copyright notice in the Description page of Project Settings.


#include "MGCharacter.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMGCharacter::AMGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsHoldingTrigger = false;

}


void AMGCharacter::PMoveCharacter(FVector2D Axis)
{
	//Create a local rotation variable
	FRotator controlRot;

	//set the yaw to the same as the controller yaw
	controlRot.Yaw = GetControlRotation().Yaw;
	
	AddMovementInput(UKismetMathLibrary::GetForwardVector(controlRot), Axis.Y);

	AddMovementInput(UKismetMathLibrary::GetRightVector(controlRot), Axis.X);
}

void AMGCharacter::HoldTrigger(bool pullOrRelease)
{
	if (HasAuthority())
	{
		Multi_OnTriggerChanged(pullOrRelease);
		return;
	}

	Server_OnTriggerChanged(pullOrRelease);
}

void AMGCharacter::Multi_OnTriggerChanged_Implementation(const bool pullOrRelease)
{
	// set is holding trigger boolean to the parameter
    		bIsHoldingTrigger = pullOrRelease;
    		OnTriggerChanged(bIsHoldingTrigger);
}

void AMGCharacter::Server_OnTriggerChanged_Implementation(const bool pullOrRelease)
{
	Multi_OnTriggerChanged(pullOrRelease);
}



