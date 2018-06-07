// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RYUCharacterBase.h"
#include "RYUCharacterIchi.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanClimbUpSignature, float, Value);

class URYUCustomizeMovementComponent;
class UClimbAssetComponent;

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

	void DebugSomething();

	/**/
	//UFUNCTION(BlueprintImplementableEvent, Category = "Climb")
	void Climb(float Val);

	UFUNCTION(BlueprintCallable)
		void SetHangUpPosition(FVector ClimUpPosition);

	UFUNCTION(BlueprintCallable)
		void ReSetHangUpPosition();

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/**Member*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerChange")
	ERYUPlayerActive PlayerActive;

	UFUNCTION()
		void ToggleAllowClimbUp();

	UPROPERTY(BlueprintAssignable, Category = "Climbing")
		FOnCanClimbUpSignature OnCanClimbUp;
	
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

	/** Called for side to side input */
	void MoveRight(float Val);

	void CanClimbDown(float Val);

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void CanClimbUp(float Val, FVector SetHangLedgePosition, FVector StartClimbUpPosition);

	void CheckClimbingLedge() override;

	void CanGrabLedgeAndClimb(float Val);

	void HangOnLedgeAndClimb(float Val);

	
	/** MEMBERS */

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FChangeActivePlayer OnPlayerActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UClimbAssetComponent* ClimbAssetComp;

	bool bAllowClimbUp;

	UPROPERTY()
		FTimerHandle AllowClimbUp_TimerHandle;

	UPROPERTY(EditAnywhere, Category = "Climbing")
		float AllowClimbUpTime;

private:

	/** METHODS */

	void InitializeCharacterValues();

	bool bDebugOutputActive;

	FVector currA;
	FVector currV;

	float DefaultGravityScale;
	float MaxGravityScaleStd;

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

	FVector HangLedgePosition;

	FVector _StartClimbUpPosition;

	bool bHangPositionSet;

	FName TraceTagFalling = "FallingDownTraceTag";

	

};