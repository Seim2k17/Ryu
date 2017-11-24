// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlateMovingComponent.h"
#include "RYUENUM_SymbolNames.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "RYUPressurePlate.generated.h"

UCLASS()
class RYU_PROTOTYPE_API ARYUPressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARYUPressurePlate();

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* SceneComp;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* BorderMesh;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* PressurePlateMesh;
	UPROPERTY(EditDefaultsOnly)
		UPlateMovingComponent* MovingPlateComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Preferences)
		TEnumAsByte<SymbolNames> Symbol;

	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerZone;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

	UFUNCTION()
		virtual void NotifyActorEndOverlap(AActor* otherActor) override;

};
