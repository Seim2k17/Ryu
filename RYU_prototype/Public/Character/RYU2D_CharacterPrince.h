// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RYU2D_CharacterBase.h"
#include "RYU2D_CharacterPrince.generated.h"

//@ToDo: look later when movement impl.
//class URYU2D_MovementComponent;
class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class RYU_PROTOTYPE_API ARYU2D_CharacterPrince : public ARYU2D_CharacterBase
{
	GENERATED_BODY()
	
/************************************************************************/
/* METHODS                                                              */
/************************************************************************/

public:
	
	ARYU2D_CharacterPrince(const class FObjectInitializer& ObjectInitializer);


	//UPROPERTY(Category = Character, VAnywhere, BlueprintReadOnly)
	//URYU2D_MovementComponent* MovementComp;

	virtual void PostInitializeComponents() override;

	void Jump() override;

	void StopJumping() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DrawDebugInfosOnScreen();

	void DebugSomething();
	
	
	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif


protected:

	void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float Val);

	/** Called for Climbing input */
	void MoveUp(float Value);

	void Climb(float Val);

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	void UpdateCharacter();

	UFUNCTION()
	void FlipbookFinishedPlaying();

	void BeginRunFlipbookFinished();

	void EndRunFlipbookFinished();

	void StartTurnFlipbookFinished();

	void PlayFlipBookAnimation(UPaperFlipbook* AnimationToPlay, bool bAnimationIsLooping);

	UFUNCTION()
	void HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



private:

	void InitializeCharacterValues();


/************************************************************************/
/* MEMBER                                                               */
/************************************************************************/

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector TurnPositionOffset;

protected:


	bool bPlayTurnAni;

	bool bStartedNoLoopAnimation;

	//**MOVEMENT*
	bool bLookRight;


private:

	float fDeltaSeconds;

	float bDebugOutputActive;

	FVector currA;
	FVector currV;

};
