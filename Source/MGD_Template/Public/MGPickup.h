// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "MGPickup.generated.h"

UCLASS()
class MGD_TEMPLATE_API AMGPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMGPickup();

	UPROPERTY(EditAnywhere, Category=Components)
	UCapsuleComponent* PickupTrigger;
	
};
