#include "CXPickupActor.h"
#include "CXPawn.h"
#include "CXPlayerState.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"


ACXPickupActor::ACXPickupActor()
{
	bReplicates = true;
	SetNetUpdateFrequency(1.0f);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->InitSphereRadius(150.0f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ACXPickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPickupOverlap);
	}
}

void ACXPickupActor::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bConsumed)
	{
		return;
	}

	const ACXPawn* VehiclePawn = Cast<ACXPawn>(OtherActor);
	if (IsValid(VehiclePawn) == false)
	{
		return;
	}

	ACXPlayerState* PS = VehiclePawn->GetPlayerState<ACXPlayerState>();
	if (IsValid(PS) == false)
	{
		return;
	}

	bConsumed = true;
	ApplyEffect(PS);
	Destroy();
}

void ACXPickupActor::Destroyed()
{
	if (GetNetMode() != NM_DedicatedServer && IsValid(GetWorld()))
	{
		if (PickupParticle)
		{
			UParticleSystemComponent* Effect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupParticle, GetActorLocation());
			if (IsValid(Effect))
			{
				FTimerHandle EffectTimer;
				GetWorld()->GetTimerManager().SetTimer(EffectTimer, FTimerDelegate::CreateUObject(Effect, &UActorComponent::DestroyComponent, false), 2.0f, false);
			}
		}

		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
	}

	Super::Destroyed();
}

