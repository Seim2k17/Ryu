// Copyright 2019 80k Games, All Rights Reserved.

/***
ReCopy to RyuMainCharacter.h/.cpp

#pragma once

#include "RYU2D_CharacterBase.h"
#include <CoreMinimal.h>
#include "RYU2D_MainCharacter.generated.h"

// WIP - V2 MainCharacter
// 1. Sort Methods and pack them to components

UCLASS()
class RYU_PROTOTYPE_API ARYU2D_MainCharacter : public ARYU2D_CharacterBase
{
    GENERATED_BODY()

public:
	
    ARYU2D_MainCharacter(const class FObjectInitializer& ObjectInitializer);

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

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void SneakPressed();

    UFUNCTION()
    void SneakReleased();

    void UpdateCharacter();

private:
    // our own functionality gets called at the end of an animation, due its a delegate function we need to mark it as UFUNCTION() with InAnimSequence parmList
    UFUNCTION()
    void AnimationSequenceEnded(const UPaperZDAnimSequence* InAnimSequence);

    void InitializeCharacterValues();

    

public:
    // most need to be moved in components
private:
    float fDeltaSeconds;

	FVector currA;
	FVector currV;

    float bDebugOutputActive;

    //*ClilmbUpVariables
    bool bHangPositionSet;

    FVector _StartClimbUpPosition;

    float MoveRightInput;

    float MoveUpInput;

    float SneakMultiplierValue;

    bool bSneakIsPressed;
};
*/
