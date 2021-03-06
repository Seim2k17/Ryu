﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/RYU2DENUM_ClimbingMode.h"
#include "Character/RYUENUM_MovementMode.h"
#include "GameFramework/Character.h"
#include "CoreMinimal.h"
#include "RYUClimbingActor.h"
#include "RYUENUM_LedgeSideEntered.h"
#include "RYUENUM_PlayerActive.h"
#include "RYUCharacterBase.generated.h"

class UUserWidget;
class USphereComponent;
class UBoxComponent;

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
    FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const
    {
        return SideViewCameraComponent;
    }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const
    {
        return CameraBoom;
    }

    void Jump() override;

    void StopJumping() override;

    UFUNCTION()
    void OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex);

    /** MEMBERS */

    UPROPERTY(EditAnywhere, Category = "Character Status")
    float Energy;

    /** Side view camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* SideViewCameraComponent;

    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** State of the Character Movement*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
    ERYUMovementMode RYUMovement;

    /** State of the Character Movement*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
    ERYUClimbingMode RYUClimbingMode;

    UPROPERTY(VisibleAnywhere, Category = "Sockets")
    FName CharacterHipSocketName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LedgeTrace")
    bool bLedgeHeightInRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
    float TreshholdYWalkRun;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* SphereTracer;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* TracerSphere;

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide);

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    FVector GetLedgeHangPosition();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    ERYULedgeSideEntered GetLedgeSideEntered();

    UFUNCTION(BlueprintCallable)
    UBoxComponent* GetOverlappedClimbingComponent(FName UpOrDown, FName LeftOrRight);

    UFUNCTION()
    void OnHandleCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                     bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

    virtual void CheckClimbingLedge();

    //DoOnce Repl.

    bool bLedgeTracePossible;

    bool bSphereTracerOverlap;

    bool bJumpJustStarted;

    FVector LedgeTracerHeight;
    FVector LedgeTracerWall;

    AActor* SphereOverlappedActor;
    UPrimitiveComponent* SphereOverlappedComponent;

    TArray<UPrimitiveComponent*> CapsuleOverlappedComponents;

    FName CanClimbUpTagName;
    FName CanClimbDownTagName;

    FName LeftPositionTagName;
    FName RightPositionTagName;

    void OnSphereTracerCheckOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp);

private:
    FVector LedgeTracerWallNormal;
    FVector HipSocketLocation;
    bool bLedgeTraceInRangeChanged;
    bool bLedgeTraceNotInRangeChanged;

    FName TraceTag = "ClimbingDownTraceTag";

    FVector LedgeHangPosition;

    ERYULedgeSideEntered ESideEntered;
};
