// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "RYUENUM_MovementMode.h"
#include "RYUENUM_ClimbingMode.h"
#include "RYU2D_CharacterBase.generated.h"

class URYU2D_AnimationComponent;
class USphereComponent;

/**
* This class is the default character for Paper2DIntro, and it is responsible for all
* physical interaction between the player and the world.
*
* The capsule component (inherited from ACharacter) handles collision with the world
* The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
* The Sprite component (inherited from APaperCharacter) handles the visuals
*/
UCLASS(config = Game)
class RYU_PROTOTYPE_API ARYU2D_CharacterBase : public APaperCharacter
{
	GENERATED_BODY()

public:

/************************************************************************/
/* METHODS                                                              */
/************************************************************************/

	ARYU2D_CharacterBase();

	ARYU2D_CharacterBase(const class FObjectInitializer& ObjectInitializer);
	
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION()
	void OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Jump() override;

	void StopJumping() override;

/************************************************************************/
/* MEMBER                                                               */
/************************************************************************/

public:

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** State of the Character Movement*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	ERYUMovementMode RYUMovement;

	/** State of the Character Climbing State */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	ERYUClimbingMode RYUClimbingMode;

protected:

	/**ANIMATIONS as extra Component*/

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URYU2D_AnimationComponent* Animation2DComponent;

	/** To look for ledges to Climb and other Stuff in front of the Character*/
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereTracer;
	
	
};
