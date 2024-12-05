// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class MGD_TEMPLATE_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

	UPROPERTY(EditAnywhere, Category=Components)
	UCapsuleComponent* PickupTrigger;

	UPROPERTY(EditAnywhere, Category=Components)
	UStaticMeshComponent* Mesh;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category=Pickup)
	TSubclassOf<AActor>PickupActorClass;

	UPROPERTY(EditDefaultsOnly, Category=Pickup)
	TArray <TSubclassOf<AActor>>IgnoreActorClass;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
