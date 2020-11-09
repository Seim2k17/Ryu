// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuMoveRightAxisInputState.h"
#include <CoreMinimal.h>
#include <Components/TimelineComponent.h>
#include "RyuMainCharacter.generated.h"

class URyuTimelineComponent;
class URyuDebugComponent;

UCLASS()
class RYU_PROTOTYPE_API ARyuMainCharacter : public ARyuBaseCharacter
{
    GENERATED_BODY()

public:
    ARyuMainCharacter(const class FObjectInitializer& ObjectInitializer);

    bool CheckFlipOverlappedActor(UBoxComponent* ClimbingTrigger);

    void Climb();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ClimbLedgeFlipBookFinished();

    void Jump() override;

    void PostInitializeComponents() override;

    void ResetCollisionAndGravity();

    void StopJumping() override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void TurnRunFlipBookFinished();

    void Tick(float DeltaTime) override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetMoveRightInput();

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetMoveUpInput();

    UFUNCTION()
    float GetFallingMoveRightMultiplier();

    float GetSneakMultiplier();

    bool GetSneakActive();

    UFUNCTION()
    FHitResult GetHitResult();

    UFUNCTION()
    void RestartCharacterAtCheckpoint(FVector& CheckpointPosition);

protected:
    // really protected ?
    void BeginPlay() override;

    void JumpUpOrDown(float Val, FVector StartJumpPosition);

    // BP exposable ?
    // UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveRight(float Val);

    void MoveUp(float Value);

    UFUNCTION()
    void HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult);

    UFUNCTION()
    void HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void SneakPressed();

    UFUNCTION()
    void SneakReleased();

    UFUNCTION()
    void SprintPressed();

    UFUNCTION()
    void SprintReleased();

    void UpdateCharacter();

private:
    void InitializeCharacterValues();

    float MoveRightKeyStatus();

    void SetDebuggedCharacter();

    // everyLineTracing Activity from MainChar will be gathered here
    void StartLineTracing();

    UFUNCTION()
    void BindToCheckpointLoaded(FVector& CheckpointPosition);

public:
    // most need to be moved in components

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bLineTracingActive = true;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bLineTracingVisible = true;

    UPROPERTY(EditAnywhere, Category = "Falling")
    float LengthLineTrace = 70.0f;

    UPROPERTY(EditAnywhere, Category = "Falling")
    float HitSphereRaduis = 20.0f;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    URyuTimelineComponent* RyuTimelineComponent;

    URyuClimbingComponent* RyuClimbingComponent;

private:
    // LineTracing / DebbugingSection
    FHitResult CharHitResult;
    FVector TraceStart;
    FVector TraceEnd;
    FCollisionQueryParams CollisionParams;
    FName TraceTag;
    TArray<AActor*> IgnoredActors;
    UWorld* TheWorld;

    float fDeltaSeconds;

    float MoveRightInput;

    float MoveUpInput;

    bool bSneakIsPressed;

    float SneakMultiplierValue;
};
