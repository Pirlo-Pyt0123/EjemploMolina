// Copyright Epic Games, Inc. All Rights Reserved.

#include "EjemploMolinaGameMode.h"
#include "EjemploMolinaPlayerController.h"
#include "EjemploMolinaCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Moneda.h"
#include "Kismet/KismetMathLibrary.h"

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

void AEjemploMolinaGameMode::BeginPlay()
{
	Super::BeginPlay();
	const int32 Cantidad = 30; // monedas
	const FVector Min(-1000.f, -1000.f, 200.f);
	const FVector Max(1000.f, 1000.f, 200.f);

	for (int32 i = 0; i < Cantidad; ++i)
	{
		FVector Pos = UKismetMathLibrary::RandomPointInBoundingBox((Min + Max) * 0.5f, (Max - Min) * 0.5f);

		FActorSpawnParameters Params;
		AMoneda* NuevaMoneda = GetWorld()->SpawnActor<AMoneda>(AMoneda::StaticClass(), Pos, FRotator::ZeroRotator, Params);

		if (NuevaMoneda)
		{
			int32 Indice = FMath::RandRange(0, 2);
			NuevaMoneda->TipoMoneda = static_cast<ETipoMoneda>(Indice);

			switch (NuevaMoneda->TipoMoneda)
			{
			case ETipoMoneda::Oro:   NuevaMoneda->Valor = 10; break;
			case ETipoMoneda::Plata: NuevaMoneda->Valor = 5; break;
			case ETipoMoneda::Cobre: NuevaMoneda->Valor = 2; break;
			}

			// 💥 ESTA ES LA LÍNEA QUE TE FALTA
			NuevaMoneda->AplicarMaterialSegunTipo();
		}
	}
}

void AEjemploMolinaGameMode::Tick(float DeltaTime)
{
}
