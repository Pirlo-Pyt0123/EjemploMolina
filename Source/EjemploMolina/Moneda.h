// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Moneda.generated.h"
UENUM(BlueprintType)

enum class ETipoMoneda : uint8
{
	Oro   UMETA(DisplayName = "Oro"),
	Plata UMETA(DisplayName = "Plata"),
	Cobre UMETA(DisplayName = "Cobre")
};

UCLASS()
class EJEMPLOMOLINA_API AMoneda : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoneda();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moneda")
	ETipoMoneda TipoMoneda;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moneda")
	int32 Valor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moneda")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Materiales")
	UMaterialInterface* MaterialOro;

	UPROPERTY(EditAnywhere, Category = "Materiales")
	UMaterialInterface* MaterialPlata;

	UPROPERTY(EditAnywhere, Category = "Materiales")
	UMaterialInterface* MaterialCobre;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AplicarMaterialSegunTipo();

};
