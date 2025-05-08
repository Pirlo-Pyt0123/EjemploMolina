// Fill out your copyright notice in the Description page of Project Settings.


#include "Bloque2.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABloque2::ABloque2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Habilitar colisiones para ser detectado por el raycast
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    CurrentMeshIndex = 0;
    CurrentMaterialIndex = 0;

    // Cargar mallas y materiales específicos para Bloque1
    CargarMallasYMateriales();

}

// Called when the game starts or when spawned
void ABloque2::BeginPlay()
{
	Super::BeginPlay();
    if (MeshOptions.Num() > 0)
    {
        MeshComponent->SetStaticMesh(MeshOptions[CurrentMeshIndex]);
    }

    if (MaterialOptions.Num() > 0)
    {
        MeshComponent->SetMaterial(0, MaterialOptions[CurrentMaterialIndex]);
    }
	
}

// Called every frame
void ABloque2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABloque2::CambiarMalla()
{
    if (MeshOptions.Num() > 0)
    {
        CurrentMeshIndex = (CurrentMeshIndex + 1) % MeshOptions.Num();
        MeshComponent->SetStaticMesh(MeshOptions[CurrentMeshIndex]);
    }
}

void ABloque2::CambiarMaterial()
{
    if (MaterialOptions.Num() > 0)
    {
        CurrentMaterialIndex = (CurrentMaterialIndex + 1) % MaterialOptions.Num();
        MeshComponent->SetMaterial(0, MaterialOptions[CurrentMaterialIndex]);
    }
}

void ABloque2::CargarMallasYMateriales()
{
    // Cambia las rutas según tus mallas y materiales específicos para Bloque1
    ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh1(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
    ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh2(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

    ConstructorHelpers::FObjectFinder<UMaterialInterface> Material1(TEXT("Material'/Game/StarterContent/Materials/M_Brick_Clay_New.M_Brick_Clay_New'"));
    ConstructorHelpers::FObjectFinder<UMaterialInterface> Material2(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'"));

    if (Mesh1.Succeeded()) MeshOptions.Add(Mesh1.Object);
    if (Mesh2.Succeeded()) MeshOptions.Add(Mesh2.Object);

    if (Material1.Succeeded()) MaterialOptions.Add(Material1.Object);
    if (Material2.Succeeded()) MaterialOptions.Add(Material2.Object);
}

