// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "OnlineSubsystemUtils.h"

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

void UMGGameInstance::OnLoginComplete(int32 localUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
 {
	 BIEOnLoginComplete(bWasSuccessful, Error);
 }
