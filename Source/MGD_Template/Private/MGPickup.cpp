// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPickup.h"

#include "MGCharacter.h"

// Sets default values
AMGPickup::AMGPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);

	// create component
	PickupTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	// set component to be the root of the class
	SetRootComponent(PickupTrigger);

	// set default size of the collision
	PickupTrigger->SetCapsuleHalfHeight(100.0f);
	PickupTrigger->SetCapsuleRadius(100.0f);

	//Creating the static mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// attach the mesh to the actor by attaching it to a component
	Mesh->SetupAttachment(RootComponent);
	
	//Move the mesh up by default
	Mesh->SetRelativeLocation(FVector(0.0f,0.0f,50.0f));

	//Turn collisions off for the mesh
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//default blah blah something i missed
	PickupActorClass = AActor::StaticClass();

	//Set default for the timer for 10 seconds
	ResetTime = 10.0f;

	MeshRotateSpeed = 50.0f;
	
}

void AMGPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority())
	{
		return;
	}	

	for (TSubclassOf<AActor>& item : IgnoreActorClass)
	{
		if (OtherActor->GetClass() == item)
		{
			return;
		}
	}
	
	if (OtherActor->GetClass()->IsChildOf(PickupActorClass))
	{
		ActivatePickup(OtherActor);
	}	
}

void AMGPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Mesh->AddLocalRotation(FRotator(0.0f, MeshRotateSpeed * DeltaSeconds,0.0f));
}

void AMGPickup::ActivatePickup(AActor* pickupActor)
{
	BP_OnActivatePickup(pickupActor);

	DeactivatePickup();

	//checking if the reset time is greater than 0
	//if (ResetTime>0.0f)
	//{
		//if so, set a timer to reactivate the pickup
		//GetWorld()->GetTimerManager().SetTimer(TH_ReactivateTimer, this, &AMGPickup::ReactivatePickup, ResetTime);
	//}	
}

void AMGPickup::ReactivatePickup_Implementation()
{
	if(HasAuthority())
	{
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	Mesh->SetVisibility(true, true);
	
}

void AMGPickup::DeactivatePickup_Implementation()
{
	if (HasAuthority())
	{
		//if so, disable the collision so it can be collected
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// regardless of server or client hide the mesh
	Mesh->SetVisibility(false, true);
}

