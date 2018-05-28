// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RYUENUM_PlayerActive.h"
#include "Character/RYUENUM_MovementMode.h"
#include "RYUCharacterBase.generated.h"


class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangeActivePlayer);

UCLASS(config = Game)
class RYU_PROTOTYPE_API ARYUCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARYUCharacterBase();

	ARYUCharacterBase(const class FObjectInitializer& ObjectInitializer);

	
	/** METHODS*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void Jump() override;

	void StopJumping() override;

	/** MEMBERS */

	UPROPERTY(EditAnywhere, Category = "Character Status")
	float Energy;
	
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/** State of the Character Movement*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	ERYUMovementMode RYUMovement;

	UPROPERTY(VisibleAnywhere, Category = "Sockets")
		FName CharacterHipSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LedgeTrace")
		bool bLedgeHeightInRange;


protected:

	/** METHODS */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	void CheckLedgeTracer();

	void TraceHeightAndWallOfLedge();

	void CheckClimbingLedge();

	//DoOnce Repl.

	bool bLedgeTracePossible;

private:

	FVector LedgeTracerHeight;
	FVector LedgeTracerWall;
	FVector LedgeTracerWallNormal;
	FVector HipSocketLocation;
	bool bLedgeTraceInRangeChanged;
	bool bLedgeTraceNotInRangeChanged;

};
