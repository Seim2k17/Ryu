// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "RyuLadderBase.generated.h"

UCLASS()
class RYU_PROTOTYPE_API ARyuLadderBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARyuLadderBase();

    float GetClimboutTreshold();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions",
              Meta = (MakeEditWidget = true))
    FVector ClimbingBottomLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions",
              Meta = (MakeEditWidget = true))
    FVector ClimbingTopLocation;

    /* distance btw. ClimbingTopLocation & top-left position of the character*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions",
              Meta = (MakeEditWidget = true))
    FVector ClimbOutTopLeftOffset;

    /* distance btw. ClimbingTopLocation & top-right position of the character*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions",
              Meta = (MakeEditWidget = true))
    FVector ClimbOutTopRightOffset;

    /* distance btw. ClimbingBottomLocation & bottom-left position of the character*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions",
              Meta = (MakeEditWidget = true))
    FVector ClimbOutBottomLeftOffset;

    /* distance btw. ClimbingBottomLocation & bottom-right position of the character*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions",
              Meta = (MakeEditWidget = true))
    FVector ClimbOutBottomRightOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    float LadderBorderThickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    float ClimbOutTreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    bool ClimbTopOutLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    bool ClimbBottomOutLeft;
};
