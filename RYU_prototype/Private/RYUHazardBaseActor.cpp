// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUHazardBaseActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/DamageType.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ARYUHazardBaseActor::ARYUHazardBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HazardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HazardMesh"));
	RootComponent = HazardMesh;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(HazardMesh);
	//HazardMesh->SetCollisionProfileName()

	HazardMesh->OnComponentBeginOverlap.AddDynamic(this, &ARYUHazardBaseActor::HandleMeshOverlap);
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUHazardBaseActor::HandleTriggerOverlap);

	HazardMesh->OnComponentEndOverlap.AddDynamic(this, &ARYUHazardBaseActor::HandleMeshEndOverlap);
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUHazardBaseActor::HandleTriggerEndOverlap);

	/** Stds */
	bPainCausing = true;
	DamagePerSec = 5.f;
	PainInterval = 0.05f;
	bUsedBoxTrigger = false;
	bPainIsOnce = false;
	bDestroyActor = false;
	DamageType = UDamageType::StaticClass();

	
}

// Called when the game starts or when spawned
void ARYUHazardBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (bUsedBoxTrigger)
	{
		HazardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else 
	{
		BoxTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (bVFXIslooping)
	{
		PlaySpecialEffect();
	}
	

}

// Called every frame
void ARYUHazardBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARYUHazardBaseActor::PainTimer()
{
	if (bPainCausing)
		{
			TSet<AActor*> TouchingActors;
			GetOverlappingActors(TouchingActors, APawn::StaticClass());

			for (AActor* const A : TouchingActors)
			{
				if (A && A->bCanBeDamaged && !A->IsPendingKill())
				{
					APawn* PawnA = Cast<APawn>(A);
					if (PawnA)
					{
						CausePainTo(A);
					}
				}
			}

			// Stop timer if nothing is overlapping us
			if (TouchingActors.Num() == 0)
			{
				GetWorldTimerManager().ClearTimer(TimerHandle_PainTimer);
			}
		}
}

void ARYUHazardBaseActor::CauseTimerPain(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Log, TEXT("Entered Trigger"));
	if (bPainCausing && OtherActor->bCanBeDamaged)
	{
		CausePainTo(OtherActor);
	}

	// Start timer if none is active
	if (!bPainIsOnce)
	{
		if (!GetWorldTimerManager().IsTimerActive(TimerHandle_PainTimer))
		{
			GetWorldTimerManager().SetTimer(TimerHandle_PainTimer, this, &ARYUHazardBaseActor::PainTimer, PainInterval, true);
		}
	}
	
}

void ARYUHazardBaseActor::PlaySpecialEffect()
{
	if (HazardFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HazardFX, GetActorLocation());
	}
}

void ARYUHazardBaseActor::PlaySoundEffect()
{
	if (HazardSFX != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HazardSFX, GetActorLocation());
	}
}


void ARYUHazardBaseActor::StopSoundEffect()
{
	UE_LOG(LogTemp, Log, TEXT("SFX DIE !!!"));
}

void ARYUHazardBaseActor::CausePainTo(class AActor* Other)
{
	if (DamagePerSec > 0.f)
	{
		TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass();
		Other->TakeDamage(DamagePerSec*PainInterval, FDamageEvent(DmgTypeClass), DamageInstigator, this);
	}

	if (bDestroyActor) this->Destroy();
}



void ARYUHazardBaseActor::HandleMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!bUsedBoxTrigger)
	{
		CauseTimerPain(OtherActor);
		if (bPainIsOnce)
		{
			PlaySpecialEffect();
		}
		PlaySoundEffect();
	}
}

void ARYUHazardBaseActor::HandleTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (bUsedBoxTrigger)
	{
		CauseTimerPain(OtherActor);
		if (bPainIsOnce)
		{
			PlaySpecialEffect();
		}
		PlaySoundEffect();
	}

}

void ARYUHazardBaseActor::HandleMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	StopSoundEffect();
}

void ARYUHazardBaseActor::HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	StopSoundEffect();
}

