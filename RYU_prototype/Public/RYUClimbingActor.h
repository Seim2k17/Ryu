// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RYUClimbingActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class ATargetPoint;

UCLASS()
class RYU_PROTOTYPE_API ARYUClimbingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARYUClimbingActor();

	void InitializeValues();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
			bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnTriggerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ClimbMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	UBoxComponent* LeftDownTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
	UBoxComponent* RightDownTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
	UBoxComponent* LeftUpTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
	UBoxComponent* RightUpTrigger;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool bLeftDownTriggerIsActive;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool bRightDownTriggerIsActive;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool bLeftUpTriggerIsActive;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool bRightUpTriggerIsActive;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChildActorComponent* LeftHangPosition;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChildActorComponent* RightHangPosition;

	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	FName CanClimbUpTag;

	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	FName CanClimbDownTag;

	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	FName LeftPositionTag;

	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	FName RightPositionTag;


	
	
};
