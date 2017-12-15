// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RYUENUM_SymbolNames.h"
#include "PushableBoxDelegatSymbol.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RYU_PROTOTYPE_API UPushableBoxDelegatSymbol : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPushableBoxDelegatSymbol();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshPreferences)
	ESymbolNames Symbol;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshPreferences)
		float sizeX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshPreferences)
		float sizeY;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshPreferences)
		float sizeZ;
	
};
