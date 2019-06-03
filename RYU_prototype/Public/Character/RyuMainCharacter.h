// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include <CoreMinimal.h>
#include <Components/TimelineComponent.h>
#include "RyuMainCharacter.generated.h"

class URyuTimelineComponent;
class URyuDebugComponent;
/**
 * WIP - V2 MainCharacter
1. Sort Methods and pack them to components
 
 */
UCLASS()
class RYU_PROTOTYPE_API ARyuMainCharacter : public ARyuBaseCharacter
{
    GENERATED_BODY()

public:
    ARyuMainCharacter(const class FObjectInitializer& ObjectInitializer);

    // register paperZD Stuff override !
    void ConfigurePlayer_Implementation(UPaperZDAnimPlayer* Player) override;

    void Jump() override;

    void PostInitializeComponents() override;

    void Tick(float DeltaTime) override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
    // really protected ?
    void BeginPlay() override;

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetMoveRightInput();

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

    void UpdateCharacter();

    URyuTimelineComponent* RyuTimelineComponent;

private:
    // our own functionality gets called at the end of an animation, due its a delegate function we need to mark it as UFUNCTION() with InAnimSequence parmList
    UFUNCTION()
    void AnimationSequenceEnded(const UPaperZDAnimSequence* InAnimSequence);

    void InitializeCharacterValues();

public:
    // most need to be moved in components
private:
    float fDeltaSeconds;

    float MoveRightInput;

    float MoveUpInput;

    bool bSneakIsPressed;
};
