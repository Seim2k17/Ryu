// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuDebugComponent.h"
#include "Enums/ERyuCharacterState.h"
#include "Character/RYU2DENUM_Movement.h"
#include "Character/RyuMainCharacter.h"
#include "Components/RyuClimbingComponent.h"
#include "Utilities/RyuStaticFunctionLibrary.h"

URyuDebugComponent::URyuDebugComponent()
{
    InitDebugValues();
}

void URyuDebugComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogRyu, Log, TEXT("Debug active."));
}

void URyuDebugComponent::DrawDebugInfosOnScreen()
{
    auto* CharOwner = URyuStaticFunctionLibrary::GetMainCharOwner(this);
    FVector currV = CharOwner->GetVelocity();
    FVector currA = CharOwner->GetCharacterMovement()->GetCurrentAcceleration();

    bool bLookRight = false;
    ERyuLookDirection LookDir = CharOwner->GetLookDirection();

    if (LookDir == ERyuLookDirection::Right)
    {
        bLookRight = true;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
                                         FString::Printf(TEXT("V(x): %s"), *currV.ToString()),
                                         false);
        GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
                                         FString::Printf(TEXT("a(x): %s"), *currA.ToString()),
                                         false);
        // now enum
        GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green,
                                         FString::Printf(TEXT("Looking Right: %s"),
                                                         bLookRight ? TEXT("true") : TEXT("false"),
                                                         false));
        GEngine->AddOnScreenDebugMessage(
            -1, 0.0f, FColor::Green,
            FString::Printf(TEXT("Is jumping: %s"),
                            CharOwner->bJumpJustStarted ? TEXT("true") : TEXT("false"), false));
    }

    FString MoveMode;
    FString ClimbMode;

    if (CharOwner == nullptr)
    {
        return;
    }
	ERyuCharacterState CharState = CharOwner->GetCharacterState();

	MoveMode = URyuStaticFunctionLibrary::CharacterStateToString(CharState);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
		FString::Printf(TEXT("Movement: %s"), *MoveMode), false);

	auto* ClimbingComp = GetOwner()->FindComponentByClass<URyuClimbingComponent>();

    if (ClimbingComp == nullptr)
    {
        return;
    }

	/* TODO for Climbing
	ClimbMode = URyuStaticFunctionLibrary::CharacterStateToString(CharState);

    switch (ClimbingComp->GetClimbingState())
    {
        case ERYUClimbingMode::NONE:
            ClimbMode = "Climbing inactive";
            break;
        case ERYUClimbingMode::HANGONLEDGE:
            ClimbMode = "HangingOnLedge";
            break;
        case ERYUClimbingMode::CLIMBDOWNLEDGE:
            ClimbMode = "ClimbingDownLedge";
            break;
        case ERYUClimbingMode::CLIMBUPLEDGE:
            ClimbMode = "ClimbingUpLedge";
            break;
        case ERYUClimbingMode::LETGOLEDGE:
            ClimbMode = "LETGOLEDGE";
            break;
        case ERYUClimbingMode::CANCLIMBDOWNLEDGE:
            ClimbMode = "CanClimbDownLedge";
            break;
        case ERYUClimbingMode::CANCLIMBUPLEDGE:
            ClimbMode = "CanClimbUpLedge";
            break;
        case ERYUClimbingMode::CANCLIMBUPANDDOWN:
            ClimbMode = "CanClimbUpAndDownLedge";
            break;
        case ERYUClimbingMode::FALLDOWNLEDGE:
            ClimbMode = "FallingDownLedge";
            break;
        case ERYUClimbingMode::CLIMBLADDERUP:
            ClimbMode = "ClimbingUpLadder";
            break;
        case ERYUClimbingMode::CLIMBLADDERDOWN:
            ClimbMode = "ClimbingDownLadder";
            break;
        default:
            break;
    }
	     
    GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
                                     FString::Printf(TEXT("ClimbingMode: %s"), *ClimbMode), false);
									 */
}

void URyuDebugComponent::OutputCapsuleOverlappedComponents(
    TArray<UPrimitiveComponent*>& ComponentArray)
{
    for (int i = 0; i < ComponentArray.Num(); i++)
    {
        UPrimitiveComponent* el = ComponentArray[i];
        UE_LOG(LogTemp, Log, TEXT("OutputCapsuleOverlapArray[%s]: %s Posi: %s Owner: %s"),
               *FString::FromInt(i), *el->GetName(), *el->GetOwner()->GetActorLocation().ToString(),
               *el->GetOwner()->GetName());
    }
}


void URyuDebugComponent::InitDebugValues()
{
    if (bActivated)
    {
        PrimaryComponentTick.bCanEverTick = true;
    }
    else
    {
        PrimaryComponentTick.bCanEverTick = false;
    }
}

void URyuDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    DrawDebugInfosOnScreen();
}

void URyuDebugComponent::PostEditChangePropertyFromOwner()
{
	UE_LOG(LogRyu, Log, TEXT("Debug Values changed."));
    InitDebugValues();
}
