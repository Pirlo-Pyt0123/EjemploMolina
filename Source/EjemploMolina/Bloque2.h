// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bloque2.generated.h"

UCLASS()
class EJEMPLOMOLINA_API ABloque2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloque2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, Category = "Opciones de Malla")
    TArray<UStaticMesh*> MeshOptions;

    UPROPERTY(EditAnywhere, Category = "Opciones de Material")
    TArray<UMaterialInterface*> MaterialOptions;

    void CambiarMalla();
    void CambiarMaterial();

private:
    int32 CurrentMeshIndex;
    int32 CurrentMaterialIndex;
    void CargarMallasYMateriales();

};
