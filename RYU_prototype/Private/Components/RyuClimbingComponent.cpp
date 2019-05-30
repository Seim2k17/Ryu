// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuClimbingComponent.h"

// Sets default values for this component's properties
URyuClimbingComponent::URyuClimbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InitializeValues();
	// ...
}


// Called when the game starts
void URyuClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FVector URyuClimbingComponent::GetLedgeHangPosition()
{
	return LedgeHangPosition;
}

/*
param posi - Position in the OverlappedComponentArray to receive the correct TriggerBox
*/
ERYULedgePosition2D URyuClimbingComponent::GetLedgePosition()
{
	ERYULedgePosition2D LedgePosi;

	// if character can Climb Up AND Down the Ledges are at Top and Bottom from the char
	if (RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN)
	{
		LedgePosi = ERYULedgePosition2D::PosiUpDown;
		CurrentClimbTagName = CanClimbUpDownName;
		UE_LOG(LogTemp, Log, TEXT("GetLedgePosition(): PositionUpAndDown"));
	}
	else
	{
		//By design we Tag the Side in Array: ComponentTags[0]
		// if there is only ony ledge to climb we need to find the correct trigger ! (CapsuleOverlappedComponents[posi])
		if (CapsuleOverlappedComponents[0]->ComponentTags[0] == CanClimbUpTagName)
		{
			LedgePosi = ERYULedgePosition2D::PosiUp;
			CurrentClimbTagName = CanClimbUpTagName;
			UE_LOG(LogTemp, Log, TEXT("GetLedgePosition(): PositionUp"));
		}
		else if (CapsuleOverlappedComponents[0]->ComponentTags[0] == CanClimbDownTagName)
		{
			LedgePosi = ERYULedgePosition2D::PosiDown;
			CurrentClimbTagName = CanClimbDownTagName;
			UE_LOG(LogTemp, Log, TEXT("GetLedgePosition(): PositionDown"));
		}
		else
		{
			UE_LOG(LogTemp, Log,
				TEXT("GetLedgeSide(): Please check if ComponentTagName[0] of Trigger %s from %s "
					"is: 'CanClimbUp' or 'CanClimbDown'."),
				*CapsuleOverlappedComponents[0]->GetName(),
				*CapsuleOverlappedComponents[0]->GetOwner()->GetName());
			LedgePosi = ERYULedgePosition2D::NONE;
		}
	}
	return LedgePosi;
}

ERYULedgeSideEntered URyuClimbingComponent::GetLedgeSideEntered()
{
	return ESideEntered;
}

ERYULedgeSideEntered URyuClimbingComponent::GetLedgeSide(int posi)
{
	//needs to be checked when getting input, the we need to decide if we flip the char or not
	ERYULedgeSideEntered LedgeSideEntered;
	//By design we Tag the Side in Array: ComponentTags[1]
	if (CapsuleOverlappedComponents[posi]->ComponentTags[1].IsEqual(LeftLedgePosiTagName))
	{
		LedgeSideEntered = ERYULedgeSideEntered::LeftSide;
		CurrentLedgePosiTagName = LeftLedgePosiTagName;
	}
	else if (CapsuleOverlappedComponents[posi]->ComponentTags[1].IsEqual(RightLedgePosiTagName))
	{
		LedgeSideEntered = ERYULedgeSideEntered::RightSide;
		CurrentLedgePosiTagName = RightLedgePosiTagName;
	}
	else
	{
		UE_LOG(LogTemp, Log,
			TEXT("GetLedgeSide(): Please check if ComponentTagName[1] of Trigger %s from %s is: "
				"'Right' or 'Left'."),
			*CapsuleOverlappedComponents[posi]->GetName(),
			*CapsuleOverlappedComponents[posi]->GetOwner()->GetName());
		LedgeSideEntered = ERYULedgeSideEntered::NONE;
	}
	return LedgeSideEntered;
}



void URyuClimbingComponent::GetOverlappingBoxComponents()
{
	GetOverlappingComponents(CapsuleOverlappedComponents);
	//please use a Lambda dude ...
	CapsuleOverlappedComponents.RemoveAll([](auto* elem) { return !elem->IsA<UBoxComponent>(); });

	//you can use this, but iterate from end to start not from start to end
	// 	for (int i = ItemArray.Num()-1; i >= 0 ; i--)
	// 	{
	// 		if (!ItemArray[i]->IsA(UBoxComponent::StaticClass()))
	// 		{
	// 			ItemArray.RemoveAt(i);
	// 		}
	// 	}
}

void URyuClimbingComponent::ToggleEnterLedgeSide()
{
	UE_LOG(LogTemp, Log, TEXT("ToggleEnterLedgeSide(): %s"), *CurrentLedgePosiTagName.ToString());
	if (CurrentLedgePosiTagName == LeftLedgePosiTagName)
	{
		CurrentLedgePosiTagName = RightLedgePosiTagName;
	}
	else
	{
		CurrentLedgePosiTagName = LeftLedgePosiTagName;
	}
}

void URyuClimbingComponent::SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide)
{
	LedgeHangPosition = LedgeTargetPoint;
	//relict from 3D CharHangPosition !
	LedgeHangPosition.Z = LedgeHangPosition.Z + 115.0f;

	//	LeftPositionTag = "Left";
	//RightPositionTag = "Right";

	if (LedgeSide == "Left")
	{
		UE_LOG(LogTemp, Log, TEXT("SetLedgeHangPosition(): Left Side Ledge."));
		ESideEntered = ERYULedgeSideEntered::LeftSide;
	}
	else
	{
		if (LedgeSide == "Right")
		{
			UE_LOG(LogTemp, Log, TEXT("SetLedgeHangPosition(): Right Side Ledge."));
			ESideEntered = ERYULedgeSideEntered::RightSide;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SetLedgeHangPosition(): no Side Ledge."));
			ESideEntered = ERYULedgeSideEntered::NONE;
		}
	}
}


void URyuClimbingComponent::InitializeValues()
{
	ESideEntered = ERYULedgeSideEntered::NONE;

	//Constant
	CanClimbUpTagName = "CanClimbUp";
	CanClimbDownTagName = "CanClimbDown";
	CanClimbUpDownName = "CanClimbUpAndDown";

	LeftLedgePosiTagName = "Left";
	RightLedgePosiTagName = "Right";
}

// Called every frame
void URyuClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

