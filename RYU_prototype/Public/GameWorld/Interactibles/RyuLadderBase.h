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

    float GetClimboutTopTreshold();

    float GetClimboutBtmTreshold();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Positions")
    FVector ClimbingBottomLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Positions")
    FVector ClimbingTopLocation;

    /* distance btw. ClimbingTopLocation & top-left position of the character*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions"/*,
              Meta = (MakeEditWidget = true)*/)
    FVector ClimbOutTopLeftOffset;

    /* distance btw. ClimbingTopLocation & top-right position of the character*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions"/*,
              Meta = (MakeEditWidget = true)*/)
    FVector ClimbOutTopRightOffset;

    /* distance btw. ClimbingBottomLocation & bottom-left position of the character*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions"/*,
              Meta = (MakeEditWidget = true)*/)
    FVector ClimbOutBottomLeftOffset;

    /* distance btw. ClimbingBottomLocation & bottom-right position of the character*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions"/*,
              Meta = (MakeEditWidget = true)*/)
    FVector ClimbOutBottomRightOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    float LadderBorderThickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    float ClimbOutTopTreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    float ClimbOutBottomTreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    float CharacterTopEnterOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    float CharacterBottomEnterOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    bool ClimbTopOutLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    bool ClimbBottomOutLeft;
};
