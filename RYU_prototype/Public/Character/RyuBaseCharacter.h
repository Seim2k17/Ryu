// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RYU2DENUM_AnimationState.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuLookDirection.h"
#include "RYU2DENUM_ClimbingMode.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"
#include "RYUENUM_LedgePosition.h"
#include "RYUENUM_LedgeSideEntered.h"
#include <CoreMinimal.h>
#include <PaperZDCharacter.h>
#include "RyuBaseCharacter.generated.h"

class USphereComponent;
class UPrimitiveComponent;
class URyuClimbingComponent;
class URyuMovementComponent;
class IRyuCharacterState;

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

public:
    ARyuBaseCharacter();

    ARyuBaseCharacter(const class FObjectInitializer& ObjectInitializer);

    /** TO Climbing-Component-Start*/

    /** TO Climbing-Component-End*/

    void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool CheckOverlapClimbableActors();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void FlipCharacter();

    UFUNCTION(BlueprintCallable, Category = "CharacterState")
    ERyuCharacterState GetCharacterState();

    // TODO Rethink after Implementing StateMachine */
    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    ERYUClimbingMode GetClimbingMode();

    ERyuLookDirection GetLookDirection();

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

    void PostInitializeComponents() override;

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    void SetClimbingMode(ERYUClimbingMode ClimbingModeToSet);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetLookRight();

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    void SetPlayerMovement(EPlayerMovement PlayerStateToSet);

    void StopJumping() override;

protected:
    virtual void BeginPlay() override;

    bool CheckOverlappingComponents();

    /** Called for side to side input */
    void MoveRight(float Val);

    //CHECK ! is it used ANYMORE ?
    void OnSphereTracerCheckOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp);

    virtual void HandleInput(ERyuInputState Input){};

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

    /** Side view camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* SideViewCameraComponent;

    /** To look for ledges to Climb and other Stuff in front of the Character*/
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* SphereTracer;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bJumpJustStarted;

    // TODO fully replace with LookDirection
    bool bLookRight;

    bool bPlayTurnAni;

    ERyuLookDirection LookDirection;

    //** Overlapped CapsuleStuff, by design we only want to overlap UBoxComp ?*/
    TArray<UPrimitiveComponent*> CapsuleOverlappedComponents;

    TArray<AActor*> CapsuleOverlappedActors;

protected:
    AActor* SphereOverlappedActor;

    UPrimitiveComponent* SphereOverlappedComponent;

    // TODO does really every Character can Climb ? I don´t think so
    URyuClimbingComponent* RyuClimbingComponent;

    URyuMovementComponent* RyuMovementComponent;

    // because we can do all the Stuff also with a equiped Item / Weapon, we need 2 FSM, else the states would explode
    // == "Concurrent State Machines"

    // TODO make it BP ready for ABP
    // need to find out how to make all of this visible to UBP, guess due Interface reasen it´s not so obvious
    //  error : UPROPERTY pointers cannot be interfaces - did you mean TScriptInterface<IRyuCharacterState>?UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
    IRyuCharacterState* CharacterState;
    IRyuCharacterState* EquipmentState;

private:
    // UtilitySection

    bool bSphereTracerOverlap;

    /** Hm why this again ? */
    bool bDoThingsOnce;

    float TreshholdYWalkRun;
};
