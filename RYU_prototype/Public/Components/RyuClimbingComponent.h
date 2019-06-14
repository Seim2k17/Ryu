// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RYU2DENUM_ClimbingMode.h"
#include "Character/RYU2DENUM_Movement.h"
#include "Enums/ERyuLedgePosition.h"
#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "RyuClimbingComponent.generated.h"

class UBoxComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RYU_PROTOTYPE_API URyuClimbingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URyuClimbingComponent();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void ChangeClimbingMode();

    void CheckMoveUpState(float MoveUpInput);

    void Climb(float MoveUpInput);

    ERYUClimbingMode GetClimbingState();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    FVector GetLedgeHangPosition();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    ERYULedgeSideEntered GetLedgeSideEntered();

    //    ERYULedgeSideEntered ARyuBaseCharacter::GetLedgeSide(int posi);
    ERYULedgeSideEntered GetLedgeSide(int posi);

    //ERYULedgePosition2D ARyuBaseCharacter::GetLedgePosition();
    ERyuLedgePosition GetLedgePosition();

    void GetOverlappingBoxComponents();

    UBoxComponent* GetOverlappedClimbingComponent(ERYULedgePosition2D LedgePosi);

    UBoxComponent* GetOverlappedClimbingComponent(FName UpOrDown, FName LeftOrRight);

    void SetClimbingPostitionsAndMovementMode(EPlayerMovement PlayerMove,
                                              UBoxComponent* ClimbingTrigger);

    void SetClimbingPositions(UBoxComponent* ClimbTrigger);

    // TODO this call will later completely only be called from Character StateMachine
    void SetClimbingState(ERYUClimbingMode ClimbState);

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide);

    void ResetClimbingState();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void ToggleEnterLedgeSide();

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

    FVector ClimbStandUpPosition;

    FVector ClimbStandDownPosition;

    FVector LedgeHangPosition;

    FVector _StartClimbUpPosition;

    //*ClilmbUpVariables
    bool bHangPositionSet;

    ERYULedgeSideEntered ESideEntered;

    bool bLedgeHeightInRange;

    /** State of the Character Climbing State */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
    // rename to: ERyuClimbingState RyuClimbingState and move to private;
    ERYUClimbingMode RYUClimbingMode;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Climbing")
    TSubclassOf<ARYUClimbingActor> ClimbableActorClass;

protected:
    void BeginPlay() override;

    void InitializeValues();

public:
    void TickComponent(float DeltaTime, ELevelTick TickType,
                       FActorComponentTickFunction* ThisTickFunction) override;

private:
};
