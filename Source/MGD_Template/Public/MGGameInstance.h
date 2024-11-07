// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "MGGameInstance.generated.h"

class IOnlineSubsystem;

/**
 * 
 */
UCLASS()
class MGD_TEMPLATE_API UMGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category=Login)
	void LoginToEOS(); 

protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Login)
	void BIEOnLoginComplete(bool success, const FString& error);
	
	// the login function that will run when the onlogincomplete delegates is run
	void OnLoginComplete(int32 localUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	//this will store a reference to the online subsystem
	const IOnlineSubsystem* OssRef;

	// this will store a reference to the identity interface in the online subsystem
	IOnlineIdentityPtr IdentityInterface;

	
};
