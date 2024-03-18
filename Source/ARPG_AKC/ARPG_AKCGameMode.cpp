// Copyright Epic Games, Inc. All Rights Reserved.

#include "ARPG_AKCGameMode.h"
#include "ARPG_AKCPlayerController.h"
#include "ARPG_AKCCharacter.h"
#include "UObject/ConstructorHelpers.h"

AARPG_AKCGameMode::AARPG_AKCGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AARPG_AKCPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}