// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RYUENUM_SymbolNames.h"
#include "PlateMovingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RYU_PROTOTYPE_API UPlateMovingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlateMovingComponent();

	UPROPERTY(EditAnywhere)
		uint16 MoveDistance;
	
	UPROPERTY(EditAnywhere)
		float MoveStep;

	UPROPERTY()
		bool MoveDown;
	UPROPERTY()
		bool MoveUp;
	UPROPERTY()
		bool ActivateSomething;

	UPROPERTY(EditDefaultsOnly)
		FVector PositionPlateOffset;

	UPROPERTY()
		uint16 ActorsOnPlate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void MovePlateDown();
	UFUNCTION(BlueprintCallable)
		void MovePlateUp();
	UFUNCTION()
		void SetTriggeredActor(AActor* otherActor);
	UFUNCTION()
		AActor* GetTriggeredActor();

private:
	
	UPROPERTY(VisibleAnywhere)
		FVector OriginLocation;

	UPROPERTY()
		UStaticMeshComponent* PressurePlateMeshComp;

	UPROPERTY()
		AActor* TriggeredActor;
};
