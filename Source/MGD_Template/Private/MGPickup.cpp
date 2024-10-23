// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPickup.h"

// Sets default values
AMGPickup::AMGPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create component
	PickupTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	// set component to be the root of the class
	SetRootComponent(PickupTrigger);

	// set default size of the collision
	PickupTrigger->SetCapsuleHalfHeight(100.0f);
	PickupTrigger->SetCapsuleRadius(100.0f);
	
	
}
