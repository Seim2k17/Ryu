// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RYU2DENUM_AnimationState.h"
#include "RYU2DENUM_ClimbingMode.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"
#include "RYUENUM_LedgePosition.h"
#include "RYUENUM_LedgeSideEntered.h"
#include <CoreMinimal.h>
#include <PaperZDCharacter.h>
#include "RyuBaseCharacter.generated.h"

class USphereComponent;

/**
* This class is the default character for Paper2DIntro, and it is responsible for all
* physical interaction between the player and the world.
*
* The capsule component (inherited from ACharacter) handles collision with the world
* The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
* The Sprite component (inherited from APaperCharacter) handles the visuals
*/

UCLASS()
class RYU_PROTOTYPE_API ARyuBaseCharacter : public APaperZDCharacter
{
    GENERATED_BODY()

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

public:
    ARyuBaseCharacter();

    ARyuBaseCharacter(const class FObjectInitializer& ObjectInitializer);

    /** TO Climbing-Component-Start*/
    UFUNCTION(BlueprintCallable, Category = "Climbing")
    FVector GetLedgeHangPosition();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    ERYULedgeSideEntered GetLedgeSideEntered();

    ERYULedgeSideEntered ARYU2D_CharacterBase::GetLedgeSide(int posi);

    ERYULedgePosition2D ARYU2D_CharacterBase::GetLedgePosition();

    void GetOverlappingBoxComponents();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide);

    /** Maybe we even can solve this better, but after the prototype or when dirty bugs occur*/
    //* Climbing Tags for Objects
    FName CanClimbUpTagName;
    FName CanClimbDownTagName;
    FName CanClimbUpDownName;
    FName CurrentClimbTagName;

    //** Position Tags for Ledges
    FName CurrentLedgePosiTagName;
    FName LeftLedgePosiTagName;
    FName RightLedgePosiTagName;

    FVector LedgeHangPosition;

    FVector ClimbStandUpPosition;

    FVector ClimbStandDownPosition;

    ERYULedgeSideEntered ESideEntered;

    bool bLedgeHeightInRange;

    /** TO Climbing-Component-End*/

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void CheckOverlappingActors();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void FlipCharacter();

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    ERYUClimbingMode GetClimbingMode();

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    EPlayerMovement GetPlayerMovement();

    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const
    {
        return CameraBoom;
    }

    /** Returns SideViewCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const
    {
        return SideViewCameraComponent;
    }

    void Jump() override;

    UFUNCTION()
    void OnHandleCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                     bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    void SetClimbingMode(ERYUClimbingMode ClimbingModeToSet);

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    void SetPlayerMovement(EPlayerMovement PlayerStateToSet);

    void StopJumping() override;

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void ToggleEnterLedgeSide();

protected:
    virtual void BeginPlay() override;

    void CheckOverlappingComponents();

    /** Called for side to side input */
    void MoveRight(float Val);

    //CHECK ! is it used ANYMORE ?
    void OnSphereTracerCheckOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp);

    void OutputCapsuleOverlappedComponents();

    /************************************************************************/
    /* MEMBER                                                               */
    /************************************************************************/

public:
    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** When StateMachine is implemented in C++ this will become a State-class
	* and there only will be ONE State To Rule them all (Animations and Logic)
	*/

    /** State of the Character Animation*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    ERYU2DAnimationState CharAnimation2DState;

    /** State of the Character Movement*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
    EPlayerMovement PlayerMovement;

    /** State of the Character Climbing State */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
    ERYUClimbingMode RYUClimbingMode;

    /** Side view camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* SideViewCameraComponent;

    /** To look for ledges to Climb and other Stuff in front of the Character*/
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* SphereTracer;

    bool bLookRight;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bJumpJustStarted;

    //** Overlapped CapsuleStuff, by design we only want to overlap UBoxComp ?*/
    TArray<UPrimitiveComponent*> CapsuleOverlappedComponents;

    TArray<AActor*> CapsuleOverlappedActors;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Climbing")
    TSubclassOf<ARYUClimbingActor> ClimbableActorClass;

    AActor* SphereOverlappedActor;

    UPrimitiveComponent* SphereOverlappedComponent;

private:
    bool bSphereTracerOverlap;

    /** Hm why this again ? */
    bool bDoThingsOnce;

    float TreshholdYWalkRun;
};
