// Copyright 2019 80k Games, All Rights Reserved.

#include "Components/RyuClimbingComponent.h"
#include "Character/RyuMainCharacter.h"
#include "Character/RYU2DENUM_Movement.h"
#include "RyuDebugComponent.h"

URyuDebugComponent::URyuDebugComponent()
{
    InitDebugValues();
}

void URyuDebugComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URyuDebugComponent::DrawDebugInfosOnScreen()
{
	auto* CharOwner = GetMainCharOwner();
	FVector currV = GetOwner()->GetVelocity();
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
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green,
			FString::Printf(TEXT("Is jumping: %s"),
				CharOwner->bJumpJustStarted ? TEXT("true")
				: TEXT("false"),
				false));
	}

	FString MoveMode;
	FString ClimbMode;
	

	if (CharOwner == nullptr)
	{
		return;
	}
	EPlayerMovement PlayerMovement = CharOwner->PlayerMovement;

	switch (PlayerMovement)
	{
	case EPlayerMovement::BEGINRUN:
		MoveMode = "BEGIN RUNNING";
		break;
	case EPlayerMovement::RUN:
		MoveMode = "RUNNING";
		break;
	case EPlayerMovement::ENDRUN:
		MoveMode = "END RUNNING";
		break;
	case EPlayerMovement::JUMPUP:
		MoveMode = "JUMPING";
		break;
	case EPlayerMovement::CANGRABLEDGE:
		MoveMode = "CanGrabLedge";
		break;
	case EPlayerMovement::CLIMBING:
		MoveMode = "Climbing";
		break;
	case EPlayerMovement::STARTTURN:
		MoveMode = "StartTurning";
		break;
	case EPlayerMovement::ENDTURN:
		MoveMode = "EndTurning";
		break;
	case EPlayerMovement::STARTFALLING:
		MoveMode = "StartFalling";
		break;
	case EPlayerMovement::FALLING:
		MoveMode = "Falling";
		break;
	case EPlayerMovement::STANDUP:
		MoveMode = "StandUp";
		break;
	case EPlayerMovement::JUMPSTART:
		MoveMode = "StartJumpFwd";
		break;
	case EPlayerMovement::JUMPLOOP:
		MoveMode = "LoopJumpFwd";
		break;
	case EPlayerMovement::JUMPEND:
		MoveMode = "EndJump";
		break;
	default:
		MoveMode = "STANDING";
	}

	auto* ClimbingComp = GetOwner()->FindComponentByClass<URyuClimbingComponent>();

	if (ClimbingComp == nullptr)
	{
		return;
	}

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
		FString::Printf(TEXT("Movement: %s"), *MoveMode), false);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
		FString::Printf(TEXT("ClimbingMode: %s"), *ClimbMode), false);
}

// Move to a BaseClass or Static UtilityLibrary !
ARyuMainCharacter* URyuDebugComponent::GetMainCharOwner()
{
    return Cast<ARyuMainCharacter>(GetOwner());
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
    InitDebugValues();
}


