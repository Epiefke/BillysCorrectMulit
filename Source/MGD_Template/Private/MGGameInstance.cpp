// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"


void UMGGameInstance::Init()
{
	Super::Init();

	//store the online subsystem
	OssRef = Online::GetSubsystem(GetWorld());

	// check if it was found and error if not
	if (!OssRef)
	{
		UE_LOG(LogTemp, Error, TEXT("Online subsystem not found."))
		return;
	}

	// store the identity interface from the oss
	IdentityInterface = OssRef->GetIdentityInterface();

	// check if the identity interface was found, error if not
	if (!IdentityInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("Identity interface not found."))
		return;
	}

	// bind to the login complete delegate in the identity interface
	IdentityInterface->OnLoginCompleteDelegates->AddUObject(this, &UMGGameInstance::OnLoginComplete);

	// store the session interface from the oss
	SessionInterface = OssRef->GetSessionInterface();

	// check if the session interface was found, error if not
	if (!SessionInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("Session interface not found"))
		return;
	}

	
}

void UMGGameInstance::LoginToEOS()
{
	if (!IdentityInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot login, identity interface not found."))
		return;
	}

	FOnlineAccountCredentials creds;
	creds.Type = TEXT("accountportal");
	
	if (!IdentityInterface->Login(0, creds))
	{
		UE_LOG(LogTemp,Error, TEXT("Login failed synchronously"))
		return;
	}

	UE_LOG(LogTemp,Warning, TEXT("Attempting to login..."))


	
}

bool UMGGameInstance::IsLoggedIn() const
{
	if (!IdentityInterface)
 			return false;
 		return IdentityInterface->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}

bool UMGGameInstance::HostGame()
{
	//check if we are logged in
	if (!IsLoggedIn())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot host game, must log in first"))
		return false;
	}

	if (!SessionInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot host game, no session interface found"))
		return false;
	}

	FOnlineSearchSettings sessionSettings;

	sessionSettings.bAllowInvites = true;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bUsesPresence = true;
	sessionSettings.NumPrivateConnections = 4;
	sessionSettings.bUseLobbiesIfAvalible = true;
	sessionSettings.bIsLANMatch = false;

	if (SessionInterface->CreateSession(0, TEXT("MGSESSION"), sessionSettings))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot host game, create session failed"))
		return false;
	}
	return true;
}



void UMGGameInstance::OnLoginComplete(int32 localUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
 {
	 BIEOnLoginComplete(bWasSuccessful, Error);
 }

void UMGGameInstance::OnCreateSessionComplete(FName sessionName, bool success, const IOnlineSubsystem* OssRef)
{
	if (!success)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create session"))
		Delegate_OnHostGame.Broadcast(false);
		return;
	}
	if (!EnableListenServer(true))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create listen server"))
		Delegate_OnHostGame.Broadcast(false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Successfully hosted session!"))
	Delegate_OnHostGame.Broadcast(true);
}
