// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShowWidgetActor.generated.h"

class UBoxComponent;
class UUserWidget;

UCLASS()
class RYU_PROTOTYPE_API AShowWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShowWidgetActor();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* TriggerBox;

	
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> WidgetToShow;

	//Variable to hold widget the widget
	UUserWidget* MissionWidgetText;

	UFUNCTION()
		void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//for functionality extended in BPs
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void MissionCompleted();

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void ShowUserWidget();

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void HideUserWidget();



	
};
