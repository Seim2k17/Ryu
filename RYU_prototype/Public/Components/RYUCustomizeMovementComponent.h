// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RYUCustomizeMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RYU_PROTOTYPE_API URYUCustomizeMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URYUCustomizeMovementComponent();

	URYUCustomizeMovementComponent(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "CharacterMovement")
		void SetGravityScaleMaximum(float GravScale);

	bool DoJump(bool bReplayingMoves) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;


public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		float AddFallingMultiplierNumber;

	/**if 0 then Char->GravityMaximumStd (3) is used*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		float GravityScaleMaximum;

	/** Jump Force added when pressing Jump from Stand (V(y) == 0*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		FVector JumpForce;

	/** Jump Force added when pressing Jump while Running*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		FVector JumpForceRun;

	/**Treshhold to activate VelocityAfterJumping, Y: vertical Velocity, Z: Falling Treshold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		FVector AfterJumpTreshold;

	/** Velocity added after Jump ends (if StartedVelocity > AfterJumpTreshold)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		FVector VelocityAfterJumping;
	
	/**Time allowed to jump when falling of a ledge*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		float CoyoteTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		bool CoyoteTimeActive;

	UFUNCTION()
		void SetNormalMaxJumpCount(int32 MaxJumps);

	UFUNCTION()
		int32 GetNormalMaxJumpCount();

	FTimerHandle Timerhandle_CoyoteTime;

private:

	int32 NormalMaxJumpCount;
};
