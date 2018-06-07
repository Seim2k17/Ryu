// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbAssetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStartPlayingMontageSignature, UAnimMontage*, Montage, FName, SectionName, bool, bReverse);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RYU_PROTOTYPE_API UClimbAssetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbAssetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "Montages")
	UAnimMontage* ClimbUpMontage;

	UPROPERTY(EditAnywhere,Category = "Montages")
	UAnimMontage* ClimbDownMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	UAnimMontage* ClimbFallMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* ClimbFallMontageShort;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* ClimbHangMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	FVector HangPositionOffset;

	UPROPERTY(VisibleAnywhere, Category = "SectionNames")
	FName ClimbDownStart;

	UPROPERTY(VisibleAnywhere, Category = "SectionNames")
	FName ClimbDownHang;

	UPROPERTY(VisibleAnywhere, Category = "SectionNames")
	FName ClimbDownEnd;

	UPROPERTY(BlueprintCallable, Category = "Climbing")
	FOnStartPlayingMontageSignature OnStartPlayingClimbDownMontage;

private:

	



		
	
};
