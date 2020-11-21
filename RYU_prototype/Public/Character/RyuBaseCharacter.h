// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RYU2DENUM_AnimationState.h"
#include "Enums/ERyuButtonType.h"
#include "Enums/ERyuCharacterPossibility.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuCharacterStatus.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuInteractionStatus.h"
#include "Enums/ERyuLookDirection.h"
#include "Enums/ERyuMoveRightAxisInputState.h"
#include "Enums/ERyuMoveUpAxisInputState.h"
#include "Enums/ERyuMovementState.h"
#include "IO/RyuBaseCommand.h"
#include "RYU2DENUM_ClimbingMode.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"
#include "RYUENUM_LedgePosition.h"
#include "RYUENUM_LedgeSideEntered.h"
#include <CoreMinimal.h>
#include <PaperZDCharacter.h>
#include <Engine/DataTable.h>
#include "RyuBaseCharacter.generated.h"

class USphereComponent;
class UPrimitiveComponent;
class URyuClimbingComponent;
class URyuMovementComponent;
class URyuCharacterState;
class UPaperZDAnimPlayer;
class UPaperZDAnimSequence;

// to remove inputs from the CurInput array when appr. press/release partas are pressed / released
USTRUCT(BlueprintType)
//USTRUCT()
struct FInputCounterparts : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(VisibleAnywhere)
    ERyuInputState PressKeyState;

    UPROPERTY(VisibleAnywhere)
    ERyuInputState ReleaseKeyState;

    UPROPERTY(VisibleAnywhere)
    ERyuButtonType ButtonType;

    bool PressStateEqualInput(ERyuInputState Input)
    {
        return (PressKeyState == Input);
    }

    bool ReleaseStateEqualInput(ERyuInputState Input)
    {
        return (ReleaseKeyState == Input);
    }
    FInputCounterparts(){};
};

USTRUCT(BlueprintType)
struct FJumpStartValues
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = "Debug")
    FVector JumpStartAcceleration;

    UPROPERTY(VisibleAnywhere, Category = "Debug")
    FVector JumpStartVelocity;
};

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

    UFUNCTION()
    void AddCurrentInputState(ERyuInputState InputStateToAdd);

    UFUNCTION()
    void CheckCurrentInputState(ERyuInputState InputStateToAdd,
                                ERyuButtonType ButtonType = ERyuButtonType::None);

    UFUNCTION()
    void RemoveInputeState(ERyuInputState InputStateToRemove);

    // our own functionality gets called at the end of an animation, due its a delegate function we need to mark it as UFUNCTION() with InAnimSequence parmList
    UFUNCTION()
    void AnimationSequenceEnded(const UPaperZDAnimSequence* InAnimSequence);

    // register paperZD Stuff override !
    void ConfigurePlayer_Implementation(UPaperZDAnimPlayer* Player) override; //*copied

    UFUNCTION(BlueprintCallable, Category = "RyuMovement")
    bool CheckOverlapClimbableActors();

    UFUNCTION()
    bool FindCurrentInputState(ERyuInputState InputState);

    /*
	* Sets the rotation of the Sprite so that the character faces his direction of travel.
	*/
    UFUNCTION(BlueprintCallable, Category = "RyuMovement")
    void FlipCharacter();

    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const
    {
        return CameraBoom;
    }

    UFUNCTION(BlueprintCallable, Category = "RyuCharacterState")
    ERyuCharacterState GetCharacterStateEnum();

    //IRyuCharacterState* GetCharacterState();
    URyuCharacterState* GetCharacterState();

    UFUNCTION(BlueprintCallable, Category = "RyuCharacterStatus")
    float GetCharacterStatus(ERyuCharacterStatus Status);

    // TODO Rethink after Implementing StateMachine */
    UFUNCTION(BlueprintCallable, Category = "RyuMovement")
    ERYUClimbingMode GetClimbingMode();

    UFUNCTION()
    TArray<ERyuInputState> GetCurrentInputStates();

    ERyuMovementState GetCharacterMovementState();

    ERyuInputState GetInputState();

    ERyuInteractionStatus GetInteractionStatus();

    UFUNCTION(BlueprintCallable, Category = "RyuState")
    ERyuCharacterState GetLastCharacterState();

    FJumpStartValues GetJumpStartValues();

    ERyuLookDirection GetLookDirection();

    ERyuMoveRightAxisInputState GetMoveRightAxisState();

    ERyuMoveUpAxisInputState GetMoveUpAxisState();

    ERyuCharacterPossibility GetCharacterPossibility();

    URyuMovementComponent* GetRyuCharacterMovement();

    float GetFallVelocityZFromJump();

    float GetFallNormalVelocityZ();

    float GetFallDeepVelocityZ();

    float GetFallToDeathVelocityZ();

    AActor* GetOverlappedActor();

    /** Returns SideViewCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const
    {
        return SideViewCameraComponent;
    }

    bool IsInCombat();

    bool IsFalling();

    bool EnemyInSight();

    virtual void HandleInput(ERyuInputState Input);

    void Jump() override;

    void JumpToAnimInstanceNode(FName Node);

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

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetMoveRightInput();

    UFUNCTION()
    void ResetEndJumpTimer();

    UFUNCTION()
    void ResetFallingTimer();

    void ResetJumpStartValues();

    void SaveJumpValues();

    UFUNCTION(BlueprintCallable, Category = "RYU Movement")
    void SetClimbingMode(ERYUClimbingMode ClimbingModeToSet);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetLookRight();

    void SetMoveRightAxisState(ERyuMoveRightAxisInputState RightAxisState);

    void StartJump();

    UFUNCTION()
    void StartJumpingTimer();

    void StopJumping() override;

    void SetLookDirection(ERyuLookDirection Direction);

    void SetLastCharacterState(ERyuCharacterState LastState);

    void SetCharacterMovementState(ERyuMovementState MovementState);

    /*ONLY FOR DEBUG/DEVELOPMENT-PURPOSE*/
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void SwitchCharacterStateEnum(ERyuCharacterState InputState);

    void SetAllowReleaseAxisKey(bool AllowState);

    void SetEndJumpTimer();

    void SetFallingTimer();

    void SetJumpAllowed(bool JumpState);

    void SetVelocityAfterJump();

protected:
    virtual void BeginPlay() override;

    bool CheckOverlappingComponents();

    //CHECK ! is it used ANYMORE ?
    void OnSphereTracerCheckOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp);

    // Function which is executed after the AllowReleaseKeyTimer
    UFUNCTION()
    void AllowReleaseKey();

    UFUNCTION()
    void IncreaseFallingVelocity();

    void InitializeCommands();

    void InitInputCounterparts();

    bool CheckCharacterEnumValue();

public:
    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** When StateMachine is implemented in C++ this will become a State-class
	* and there only will be ONE State To Rule them all (Animations and Logic)
	*/

    // TODO: do we really need this after STM-Integration ?
    /** State of the Character Animation */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    ERYU2DAnimationState CharAnimation2DState;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    FVector DuckSpriteRelativePosition;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    float DuckCapsuleHeight = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    float IdleCapsuleHeight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    FVector IdleSpriteRelativePosition;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    FName IdleNodeName = TEXT("ToIdle");

    // TODO make a Map out of it
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    FName JumpNodeName = TEXT("Jumping");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    FName RunNodeName = TEXT("ToRun");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    FName WalkNodeName = TEXT("ToWalk");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    FName FallinghNodeName = TEXT("ToFalling");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaperZD")
    FName ClimbinghNodeName = TEXT("ToClimbing");

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

    //** Overlapped CapsuleStuff, by design we only want to overlap UBoxComp ?*/
    TArray<UPrimitiveComponent*> CapsuleOverlappedComponents;

    TArray<AActor*> CapsuleOverlappedActors;

protected:
    AActor* SphereOverlappedActor;

    UPrimitiveComponent* SphereOverlappedComponent;

    URyuMovementComponent* RyuMovementComponent;

    // because we can do all the Stuff also with a equiped Item / Weapon, we need 2 FSM, else the states would explode
    // == "Concurrent State Machines"

    // TODO make it BP ready for ABP
    // need to find out how to make all of this visible to UBP, guess due Interface reasen it´s not so obvious
    //  error : UPROPERTY pointers cannot be interfaces - did you mean TScriptInterface<IRyuCharacterState>?UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
    UPROPERTY()
    //IRyuCharacterState* CharacterState;
    URyuCharacterState* CharacterState;

    ERyuCharacterPossibility CharacterPosibility;

    UPROPERTY()
    //IRyuCharacterState* EquipmentState;
    URyuCharacterState* EquipmentState;

    FTimerHandle AllowReleaseKeyTimerHandle;

    // DataStructure for all CommandsButtons / 4CommandDesignPattern
    UPROPERTY(VisibleAnywhere)
    TMap<FString, URyuBaseCommand*> Commands;

    UPROPERTY(VisibleAnywhere, Category = "Input")
    TArray<ERyuInputState> CurrentInputs;

    UPROPERTY(VisibleAnywhere, Category = "Input")
    TMap<ERyuInputState, FInputCounterparts> KeyInputCounterpartMap;

    // linked Datatable with KeyCounterparts
    UPROPERTY(EditAnywhere, Category = "Input")
    UDataTable* KeyInputCounterpartTable;

    // after which time we allow the ReleaseKeyEvent for a certain Axis (ReleaseKey is bad for quick directionChanges)
    float AllowReleaseAxisKeyTime = 0.5f;
    bool bAllowReleaseAxisKey = true;

    UPROPERTY(VisibleAnywhere, Category = "Debug")
    FJumpStartValues JumpStartValues;

private:
    ERyuCharacterState LastCharacterState = ERyuCharacterState::None;

    // UtilitySection
    ERyuLookDirection LookDirection = ERyuLookDirection::Right;

    ERyuInteractionStatus InteractionStatus = ERyuInteractionStatus::None;
    ERyuMovementState CharacterMovementState = ERyuMovementState::Standing;

    bool bSphereTracerOverlap;

    /** Hm why this again ? */
    bool bDoThingsOnce;

    float TreshholdYWalkRun;

    FInputCounterparts* PressedState;

    ERyuMoveRightAxisInputState MoveRightAxisState;

    ERyuMoveUpAxisInputState MoveUpAxisState;

    bool bHandleInput = false;

    FTimerHandle EndJumpTimerHandle;

    FTimerHandle FallingTimerHandle;
};
