// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameModeBase.h"


UMGGameInstance::UMGGameInstance()
{
	GameHasStarted = false;
	OssRef = nullptr;
	
}

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

	// bind to on create session complete
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMGGameInstance::OnCreateSessionComplete);

	// bind to on inviter accepted
	SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &UMGGameInstance::OnAcceptSessionInvite);

	// bind to on join session complete
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMGGameInstance::OnJoinSessionComplete);
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

	// return true if login status is logged in
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

	FOnlineSessionSettings sessionSettings;
	
	sessionSettings.bAllowInvites = true;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bUsesPresence = true;
	sessionSettings.NumPrivateConnections = 4;
	sessionSettings.bUseLobbiesIfAvailable = true;
	sessionSettings.bIsLANMatch = false;

	if (SessionInterface->CreateSession(0, TEXT("MGSESSION"), sessionSettings))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot host game, create session failed"))
		return false;
	}
	return true;
}

void UMGGameInstance::StartGame()
{
	if (!IsLoggedIn())
		return;

	if (!SessionInterface)
		return;

	GameHasStarted = true;

	if (SessionInterface->GetSessionState(TEXT("MGSESSION")) == EOnlineSessionState::NoSession)
		return;

	GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;

	// "/Game/MyContent/Maps/Lvl_Test?listen?GameMode=?Script/MGD_Template.GM_Battle"
	GetWorld()->ServerTravel("/Game/MyContent/Maps/Lvl_Gameplay?listen?GameMode=?Script?MGD_Template.GM_SpookyMaze");
}

void UMGGameInstance::TravelToLobby()
{
	if (!IsLoggedIn())
		return;

	if (!SessionInterface)
		return;

	GameHasStarted = false;

	if (SessionInterface->GetSessionState(TEXT("MGSESSION")) == EOnlineSessionState::NoSession)
		return;

	GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;

	// "/Game/MyContent/Maps/Lvl_Test?listen?GameMode=?Script/MGD_Template.GM_Battle"
	GetWorld()->ServerTravel("/Game/MyContent/Maps/Lvl_Lobby?listen?GameMode=?Script?MGD_Template.GM_Lobby");
}

void UMGGameInstance::TravelToGhostWin()
{
	if (!IsLoggedIn())
		return;

	if (!SessionInterface)
		return;
	
	GameHasStarted = false;

	if (SessionInterface->GetSessionState(TEXT("MGSESSION")) == EOnlineSessionState::NoSession)
		return;

	GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;

	// "/Game/MyContent/Maps/Lvl_Test?listen?GameMode=?Script/MGD_Template.GM_Battle"
	GetWorld()->ServerTravel("/Game/MyContent/Maps/Lvl_GhostWins?listen?GameMode=?Script?MGD_Template.GM_Lobby");
}

void UMGGameInstance::TravelToHumansWin()
{
	if (!IsLoggedIn())
		return;

	if (!SessionInterface)
		return;
	
	GameHasStarted = false;

	if (SessionInterface->GetSessionState(TEXT("MGSESSION")) == EOnlineSessionState::NoSession)
		return;

	GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;

	// "/Game/MyContent/Maps/Lvl_Test?listen?GameMode=?Script/MGD_Template.GM_Battle"
	GetWorld()->ServerTravel("/Game/MyContent/Maps/Lvl_HumansWin?listen?GameMode=?Script?MGD_Template.GM_Lobby");
}

bool UMGGameInstance::IsInSession() const
{
	if (!SessionInterface)
 		return false;
 
 	return SessionInterface->GetSessionState(TEXT("MGSESSION")) != EOnlineSessionState::NoSession;
}

void UMGGameInstance::OnLoginComplete(int32 localUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
 {
	 BIEOnLoginComplete(bWasSuccessful, Error);
 }

void UMGGameInstance::OnCreateSessionComplete(FName sessionName, bool success)
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

	// void UMGGameInstance::OnCreateSessionComplete(FName sessionName, bool success, const IOnlineSubsystem* OssRef);

void UMGGameInstance::OnAcceptSessionInvite(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId,
	const FOnlineSessionSearchResult& InviteResult)
{
		if (!bWasSuccessful)
    	{
    		UE_LOG(LogTemp, Error, TEXT("Could not accept session invite"))
    		return;
    	}
    
    	if (!SessionInterface)
    	{
    		UE_LOG(LogTemp, Error, TEXT("Could not accept session invite, no session interface"))
    		return;
    	}
    
    	// join the session
    	if (SessionInterface->JoinSession(0, TEXT("MGSESSION"), InviteResult))
    	{
    		UE_LOG(LogTemp, Error, TEXT("Could not join session"))
    		return;
    	}
    
    	UE_LOG(LogTemp, Warning, TEXT("Attempting to join session"))
}

void UMGGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to join session"))
		return;
	}

	ClientTravelToSession(0, sessionName);
	UE_LOG(LogTemp, Warning, TEXT("Successfully joined and travelling to session"))
}
