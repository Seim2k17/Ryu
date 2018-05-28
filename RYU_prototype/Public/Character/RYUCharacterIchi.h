// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RYUCharacterBase.h"
#include "RYUCharacterIchi.generated.h"

/**
 * 
 */

class URYUCustomizeMovementComponent;
class USphereComponent;

UCLASS()
class RYU_PROTOTYPE_API ARYUCharacterIchi : public ARYUCharacterBase
{
	GENERATED_BODY()
	
public:

	//ARYUCharacterIchi();

	ARYUCharacterIchi(const class FObjectInitializer& ObjectInitializer);
	

	//UPROPERTY(Category = Character, VAnywhere, BlueprintReadOnly)
	URYUCustomizeMovementComponent* CustMovementComp;

	virtual void PostInitializeComponents() override;
	
	void Jump() override;

	void StopJumping() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DrawDebugInfosOnScreen();


#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/**Member*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerChange")
		ERYUPlayerActive PlayerActive;
	

	UFUNCTION()
		void OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AfterJumpButtonPressed();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION()
		void ChangePlayer();

	UFUNCTION()
		void DeactivateCoyotoeJumpPossible();

	/** MEMBERS */

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereTracer;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FChangeActivePlayer OnPlayerActive;

	/** Called for side to side input */
	void MoveRight(float Val);


private:

	/** METHODS */

	void InitializeCharacterValues();

	bool bDebugOutputActive;

	FVector currA;
	FVector currV;

	float DefaultGravityScale;
	float MaxGravityScaleStd;

	bool bJumpJustStarted;

	float CharMaxWalkSpeed;

	float currentFPS;

	float StartJumpZVelocity;
	FVector StartJumpVelocity;

	bool tmpCheck;
	bool FallCheck;

	//4 calc JumpTimeNeeded
	float TimeDeltaStart;
	float TimeDeltaEnd;

	//CoyoteTimeJump
	bool CoyoteJumpPossible;

	//Position when Jumping
	FVector StartJumpPosition;
};