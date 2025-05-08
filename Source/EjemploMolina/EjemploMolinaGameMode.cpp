// Copyright Epic Games, Inc. All Rights Reserved.

#include "EjemploMolinaGameMode.h"
#include "EjemploMolinaPlayerController.h"
#include "EjemploMolinaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEjemploMolinaGameMode::AEjemploMolinaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AEjemploMolinaPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}