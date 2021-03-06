// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/RYU2DENUM_AnimationState.h"
#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "RYU2DENUM_ClimbingMode.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"
#include "RYUENUM_LedgePosition.h"
#include "RYUENUM_LedgeSideEntered.h"
#include "RYU2D_CharacterBase.generated.h"

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
class RYU_PROTOTYPE_API ARYU2D_CharacterBase : public APaperZDCharacter
{
    GENERATED_BODY()

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

public:
    ARYU2D_CharacterBase();

    ARYU2D_CharacterBase(const class FObjectInitializer& ObjectInitializer);

    /** Returns SideViewCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const
    {
        return SideViewCameraComponent;
    }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const
    {
        return CameraBoom;
    }

    UFUNCTION()
    void OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex);

    void Jump() override;

    void StopJumping() override;

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide);

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    FVector GetLedgeHangPosition();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    ERYULedgeSideEntered GetLedgeSideEntered();

    UFUNCTION()
    void OnHandleCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                     bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void CheckOverlappingActors();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void FlipCharacter();

    //ERYULedgeSideEntered ARYU2D_CharacterBase::GetLedgeSide(int posi);
	ERYULedgeSideEntered GetLedgeSide(int posi);

    //ERYULedgePosition2D ARYU2D_CharacterBase::GetLedgePosition();
	ERYULedgePosition2D GetLedgePosition();

    void GetOverlappingBoxComponents();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void ToggleEnterLedgeSide();

protected:
    virtual void BeginPlay() override;

    /** Called for side to side input */
    void MoveRight(float Val);

    //CHECK ! is it used ANYMORE ?
    void OnSphereTracerCheckOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp);

    void CheckOverlappingComponents();

    void OutputCapsuleOverlappedComponents();

    /************************************************************************/
    /* MEMBER                                                               */
    /************************************************************************/

public:
    /** Side view camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* SideViewCameraComponent;

    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** State of the Character Movement*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
    EPlayerMovement PlayerMovement;

    /** State of the Character Animation*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    ERYU2DAnimationState CharAnimation2DState;

    /** State of the Character Climbing State */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
    ERYUClimbingMode RYUClimbingMode;

    /** To look for ledges to Climb and other Stuff in front of the Character*/
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* SphereTracer;

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    void SetPlayerMovement(EPlayerMovement PlayerStateToSet);

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    EPlayerMovement GetPlayerMovement();

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    void SetClimbingMode(ERYUClimbingMode ClimbingModeToSet);

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    ERYUClimbingMode GetClimbingMode();

    bool bLookRight;

protected:
    //* Climbing Tags for Objects
    FName CanClimbUpTagName;
    FName CanClimbDownTagName;
    FName CanClimbUpDownName;
    FName CurrentClimbTagName;

    //** Position Tags for Ledges
    FName LeftLedgePosiTagName;
    FName RightLedgePosiTagName;
    FName CurrentLedgePosiTagName;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bJumpJustStarted;

    //** Overlapped CapsuleStuff, by design we only want to overlap UBoxComp ?*/
    TArray<UPrimitiveComponent*> CapsuleOverlappedComponents;

    TArray<AActor*> CapsuleOverlappedActors;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Climbing")
    TSubclassOf<ARYUClimbingActor> ClimbableActorClass;

    AActor* SphereOverlappedActor;
    UPrimitiveComponent* SphereOverlappedComponent;

    FVector LedgeHangPosition;

    FVector ClimbStandUpPosition;

    FVector ClimbStandDownPosition;

    ERYULedgeSideEntered ESideEntered;

private:
    bool bSphereTracerOverlap;

    bool bDoThingsOnce;

    float TreshholdYWalkRun;

    bool bLedgeHeightInRange;
};
