// Fill out your copyright notice in the Description page of Project Settings.


#include "Moneda.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Components/PrimitiveComponent.h"
#include "EjemploMolinaCharacter.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMoneda::AMoneda()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshMoneda"));
	RootComponent = Mesh;

	// Cargar Static Mesh desde contenido
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/coin/Coin.Coin'"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}

	//colisiones
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMoneda::OnBeginOverlap);

	// Cargar Materiales
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatOro(TEXT("Material'/Game/coin/GoldCoin.GoldCoin'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatPlata(TEXT("Material'/Game/StarterContent/Materials/SilverCoin.SilverCoin'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatCobre(TEXT("Material'/Game/StarterContent/Materials/CobreCoin.CobreCoin'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> SonidoPickUp(TEXT("SoundWave'/Game/StarterContent/Audio/coin.coin'"));
	if (SonidoPickUp.Succeeded())
	{
		SonidoAlRecoger = SonidoPickUp.Object;
	}

	if (MatOro.Succeeded())   MaterialOro = MatOro.Object;
	if (MatPlata.Succeeded()) MaterialPlata = MatPlata.Object;
	if (MatCobre.Succeeded()) MaterialCobre = MatCobre.Object;

	// Valor por defecto
	TipoMoneda = ETipoMoneda::Oro;
	Valor = 10;

}

// Called when the game starts or when spawned
void AMoneda::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AMoneda::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Rotar sobre Z
	FRotator Rotacion = FRotator(0.f, 90.f * DeltaTime, 0.f); 
	AddActorLocalRotation(Rotacion);

}

void AMoneda::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEjemploMolinaCharacter* Personaje = Cast<AEjemploMolinaCharacter>(OtherActor);
	if (Personaje)
	{
		Personaje->RecolectarMoneda(TipoMoneda, Valor);

		if (SonidoAlRecoger)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SonidoAlRecoger, GetActorLocation());
		}

		Destroy();
	}
}

void AMoneda::AplicarMaterialSegunTipo()
{
	switch (TipoMoneda)
	{
	case ETipoMoneda::Oro:
		if (MaterialOro) Mesh->SetMaterial(0, MaterialOro);
		break;
	case ETipoMoneda::Plata:
		if (MaterialPlata) Mesh->SetMaterial(0, MaterialPlata);
		break;
	case ETipoMoneda::Cobre:
		if (MaterialCobre) Mesh->SetMaterial(0, MaterialCobre);
		break;
	}
}


