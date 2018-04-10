// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RYUHazardBaseActor.generated.h"


class UStaticMeshComponent;
class UBoxComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;

UCLASS()
class RYU_PROTOTYPE_API ARYUHazardBaseActor : public AActor
{
	GENERATED_BODY()
	
/** METHODS */

public:	
	// Sets default values for this actor's properties
	ARYUHazardBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** damage overlapping actors if pain causing. */
	void CausePainTo(class AActor* Other);

	UFUNCTION()
	void HandleMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void HandleTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void HandleMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Damage overlapping actors if pain-causing. */
	virtual void PainTimer();

	/** Causes Pain in a special Interval*/
	void CauseTimerPain(AActor* OtherActor);

	/** Play a VFX */
	void PlaySpecialEffect();

	/** Play a SFX */
	void PlaySoundEffect();

	/** Stop the PainCausingSound*/
	void StopSoundEffect();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Whether volume currently causes damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bPainCausing;
	/** Damage done per second to actors in this volume when bPainCausing=true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		float DamagePerSec;
	/** If pain causing, time between damage applications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		float PainInterval;
	/** if bPainCausing, causes pain only once (e.g. explosion*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bPainIsOnce;
	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bDestroyActor;
	/** Mesh used for visual representation */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* HazardMesh;
	/** VFX is looping or not ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		bool bVFXIslooping;
	/** Special FX which plays when Actor enters Hazard*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* HazardFX;
	/** Sound FX when damaged*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		USoundBase* HazardSFX;
	/** if bUsedBoxTrigger then a custom BoxTrigger is used*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bUsedBoxTrigger;
	/** Custom BoxTrigger used for collision if bUsedBoxTrigger = true*/
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* BoxTrigger;
	/** Controller that gets credit for any damage caused by this Actor */
	UPROPERTY()
		class AController* DamageInstigator;
	/** Type of damage done */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		TSubclassOf<UDamageType> DamageType;
	
protected:

	/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;

	
	
};
