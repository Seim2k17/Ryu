// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "RyuClimbingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RYU_PROTOTYPE_API URyuClimbingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URyuClimbingComponent();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    FVector GetLedgeHangPosition();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    ERYULedgeSideEntered GetLedgeSideEntered();

    //    ERYULedgeSideEntered ARyuBaseCharacter::GetLedgeSide(int posi);
    ERYULedgeSideEntered GetLedgeSide(int posi);

    //ERYULedgePosition2D ARyuBaseCharacter::GetLedgePosition();
    ERYULedgePosition2D GetLedgePosition();

    void GetOverlappingBoxComponents();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide);

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

    FVector LedgeHangPosition;

    FVector ClimbStandUpPosition;

    FVector ClimbStandDownPosition;

    ERYULedgeSideEntered ESideEntered;

    bool bLedgeHeightInRange;

protected:
    void BeginPlay() override;

    void InitializeValues();

public:
    void TickComponent(float DeltaTime, ELevelTick TickType,
                       FActorComponentTickFunction* ThisTickFunction) override;
};
