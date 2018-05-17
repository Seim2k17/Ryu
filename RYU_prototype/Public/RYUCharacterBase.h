// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RYUENUM_PlayerActive.h"
#include "RYUCharacterBase.generated.h"


class UUserWidget;
class URYUCustomizeMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangeActivePlayer);

UCLASS(config = Game)
class RYU_PROTOTYPE_API ARYUCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARYUCharacterBase();

	
	/** METHODS*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DrawDebugInfosOnScreen();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void Jump() override;

	void StopJumping() override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/** MEMBERS */

	UPROPERTY(EditAnywhere, Category = "Character Status")
	float Energy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerChange")
	ERYUPlayerActive PlayerActive;
	
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	URYUCustomizeMovementComponent* CustMovementComp;


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

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UFUNCTION()
	void ChangePlayer();

	/** MEMBERS */

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FChangeActivePlayer OnPlayerActive;


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
	
};
