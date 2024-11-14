// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

#include "MGGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHostGame, bool, success);

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

	UFUNCTION(BlueprintPure, Category=Login)
	bool IsLoggedIn() const;

	UFUNCTION(BlueprintCallable, Category=Session)
	bool HostGame();

	UPROPERTY(BluePrintAssignable,Category=Session)
	FOnHostGame Delegate_OnHostGame;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Login)
	void BIEOnLoginComplete(bool success, const FString& error);
	
	// the login function that will run when the onlogincomplete delegates is run
	void OnLoginComplete(int32 localUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	// the function that will run when
	void OnCreateSessionComplete(FName sessionName, bool success);

	//void OnAcceptSessionInvite

	//this will store a reference to the online subsystem
	const IOnlineSubsystem* OssRef;

	// this will store a reference to the identity interface in the online subsystem
	IOnlineIdentityPtr IdentityInterface;

	// this will store a reference to the session interface for running session based code
	IOnlineSessionPtr SessionInterface; 
};
