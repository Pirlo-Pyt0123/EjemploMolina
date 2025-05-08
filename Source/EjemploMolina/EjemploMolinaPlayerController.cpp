// Copyright Epic Games, Inc. All Rights Reserved.

#include "EjemploMolinaPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "EjemploMolinaCharacter.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Bloque.h"
#include "Bloque2.h"

AEjemploMolinaPlayerController::AEjemploMolinaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AEjemploMolinaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AEjemploMolinaPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AEjemploMolinaPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AEjemploMolinaPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AEjemploMolinaPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AEjemploMolinaPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AEjemploMolinaPlayerController::OnResetVR);


	// Asignar teclas para cambiar mallas y materiales
	InputComponent->BindAction("CambiarMalla", IE_Pressed, this, &AEjemploMolinaPlayerController::CambiarMallaBloque);
	InputComponent->BindAction("CambiarMaterial", IE_Pressed, this, &AEjemploMolinaPlayerController::CambiarMaterialBloque);


}

void AEjemploMolinaPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AEjemploMolinaPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AEjemploMolinaCharacter* MyPawn = Cast<AEjemploMolinaCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AEjemploMolinaPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AEjemploMolinaPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AEjemploMolinaPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AEjemploMolinaPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AEjemploMolinaPlayerController::CambiarMallaBloque()
{
	TArray<AActor*> BloquesCercanos = DetectarBloquesCercanos();
	for (AActor* Bloque : BloquesCercanos)
	{
		if (ABloque* Bloque1 = Cast<ABloque>(Bloque))
		{
			Bloque1->CambiarMalla();
		}
		else if (ABloque2* Bloque2 = Cast<ABloque2>(Bloque))
		{
			Bloque2->CambiarMalla();
		}
	}
}

void AEjemploMolinaPlayerController::CambiarMaterialBloque()
{
	TArray<AActor*> BloquesCercanos = DetectarBloquesCercanos();
	for (AActor* Bloque : BloquesCercanos)
	{
		if (ABloque* Bloque1 = Cast<ABloque>(Bloque))
		{
			Bloque1->CambiarMaterial();
		}
		else if (ABloque2* Bloque2 = Cast<ABloque2>(Bloque))
		{
			Bloque2->CambiarMaterial();
		}
	}
}

TArray<AActor*> AEjemploMolinaPlayerController::DetectarBloquesCercanos()
{
	TArray<AActor*> BloquesDetectados;

	// Obtener posición del jugador
	APawn* PlayerPawn = GetPawn();
	if (!PlayerPawn)
	{
		return BloquesDetectados;
	}

	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	float Radio = 500.0f;  // Radio de detección

	// Configurar parámetros de la esfera
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radio);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerPawn);  // Ignorar al jugador

	// Realizar detección esférica
	bool bOverlap = GetWorld()->OverlapMultiByChannel(Overlaps, PlayerLocation, FQuat::Identity, ECC_Visibility, CollisionShape, QueryParams);

	if (bOverlap)
	{
		for (const FOverlapResult& Result : Overlaps)
		{
			AActor* OverlappedActor = Result.GetActor();
			if (OverlappedActor && (OverlappedActor->IsA(ABloque::StaticClass()) || OverlappedActor->IsA(ABloque2::StaticClass())))
			{
				// Agregar a la lista de bloques detectados
				BloquesDetectados.Add(OverlappedActor);
			}
		}
	}

	return BloquesDetectados;
}
