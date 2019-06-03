// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuTimelineComponent.h"

// Sets default values for this component's properties
URyuTimelineComponent::URyuTimelineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void URyuTimelineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URyuTimelineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URyuTimelineComponent::TimelineFinishedCallback()
{
	// This function is called when the timeline finishes playing.

	//SetActorLocation(Curve2DComponent->ClimbUpEndTimelineLocation);

	//we use this to set back the movementmode now we go to the fun stuff the real climbing !
	auto* RyuMovementComp = GetOwner()->FindComponentByClass<URyuMovementComponent>();

	if ( RyuMovementComp == false)
	{
		return;
	}
	else
	{
		switch (RyuMovementComp->GetPlayerMovement())
		{
		case EPlayerMovement::JUMPUP:
			RyuMovementComp->SetMovementMode(MOVE_Walking);
			break;
		default:
			break;
		}

		switch (RYUClimbingMode)
		{
		case ERYUClimbingMode::NONE:
			break;
		case ERYUClimbingMode::CANCLIMBUPLEDGE:
			break;
		case ERYUClimbingMode::CANCLIMBDOWNLEDGE:
			break;
		case ERYUClimbingMode::CANCLIMBUPANDDOWN:
			break;
		case ERYUClimbingMode::JUMPTOLEDGE:
			break;
		case ERYUClimbingMode::CLIMBDOWNLEDGE:
			break;
		case ERYUClimbingMode::CLIMBUPLEDGE:
			RyuMovementComponent->SetMovementMode(MOVE_Walking);
			break;
		case ERYUClimbingMode::FALLDOWNLEDGE:
			break;
		case ERYUClimbingMode::HANGONLEDGE:
			break;
		case ERYUClimbingMode::LETGOLEDGE:
			break;
		case ERYUClimbingMode::CANENTERLADDER:
			break;
		case ERYUClimbingMode::CLIMBLADDERUP:
			break;
		case ERYUClimbingMode::CLIMBLADDERDOWN:
			break;
		default:
			break;
		}
	}

	
}


void URyuTimelineComponent::TimelineCallbackFloat(float val)
{
	// This function is called for every tick when the timeline runs.
	//UE_LOG(LogTemp, Log, TEXT("TimelineCallbackFloat(): Here incr. z-Location: %s"), *FString::SanitizeFloat(val));

	FVector StartTLLocation;
	FVector EndTLLocation;

	StartTLLocation = Curve2DComponent->StartTimelineLocation;
	EndTLLocation = Curve2DComponent->EndTimelineLocation;

	//Switch PMove needable ? ? ? or are all timelines handled the same ?
	switch (PlayerMovement)
	{
	case EPlayerMovement::JUMPUP:
		//TODO: look for ClimbUp vs. Down etc // see mix from 3D LedgeClimbing but wo Rootmotion ...

		break;
	default:
		break;
	}

	SetActorLocation(FMath::Lerp(StartTLLocation, EndTLLocation, val));
	//AddActorLocalOffset(FVector(0,0,CurrentTimeline->));
}

void URyuTimelineComponent::TimelineCallbackVector(FVector Vec)
{
	// This function is called for every tick in the timeline.
	//UE_LOG(LogTemp, Log, TEXT("TimelineCallbackVector(): Here incr. z-Location: %s"), *FString::SanitizeFloat(val));

	FVector StartTLLocation;
	FVector EndTLLocation;

	StartTLLocation = Curve2DComponent->StartTimelineLocation;
	EndTLLocation = Curve2DComponent->EndTimelineLocation;

	//Switch PMove needable ? ? ? or are all timelines handled the same ?
	switch (PlayerMovement)
	{
	case EPlayerMovement::JUMPUP:
		//TODO: look for ClimbUp vs. Down etc // see mix from 3D LedgeClimbing but wo Rootmotion ...

		break;
	default:
		break;
	}

	SetActorLocation(FMath::Lerp(StartTLLocation, EndTLLocation, Vec));
	//AddActorLocalOffset(FVector(0,0,CurrentTimeline->));
}



void URyuTimelineComponent::PlayTimeline()
{
	if (CurrentTimeline != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("PlayTimeline(): Play a timeline from Start"));
		CurrentTimeline->PlayFromStart();
	}
}

void URyuTimelineComponent::SetCurrentTimelineParamsFloat(UCurveFloat* FloatCurveX,
	UCurveFloat* FloatCurveZ,
	bool TimelineIsLooping,
	bool IgnoreTimeDilation)
{
	if (FloatCurveX)
	{
		UE_LOG(LogTemp, Log,
			TEXT("SetCurrentTimelineParamsFloat(): Set TimeLine Params for Timeline X %s"),
			*FloatCurveX->GetName());
		CurrentTimeline->AddInterpFloat(FloatCurveX, onTimelineCallbackFloat, FName("MovementX"));
	}

	if (FloatCurveZ)
	{
		UE_LOG(LogTemp, Log,
			TEXT("SetCurrentTimelineParamsFloat(): Set TimeLine Params for Timeline Z %s"),
			*FloatCurveZ->GetName());
		//Add Float curve to the timeline and bind it to the interpfunction´s delegate
		//3rd Parameter = floatValue, Propertyname, Bind all Stuff

		CurrentTimeline->AddInterpFloat(FloatCurveZ, onTimelineCallbackFloat, FName("MovementZ"));
	}

	CurrentTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

	// Timeline Settings
	CurrentTimeline->SetLooping(TimelineIsLooping);
	//false = it moves as the global Time Dileation
	CurrentTimeline->SetIgnoreTimeDilation(IgnoreTimeDilation);
}

void URyuTimelineComponent::SetCurrentTimelineParamsVector(UCurveVector* VectorCurve,
	bool TimelineIsLooping,
	bool IgnoreTimeDilation)
{
	if (VectorCurve)
	{
		UE_LOG(LogTemp, Log,
			TEXT("SetCurrentTimelineParamsVector(): Set TimeLine Params for Timeline %s"),
			*VectorCurve->GetName());
		//Add Float curve to the timeline and bind it to the interpfunction´s delegate
		//3rd Parameter = floatValue, Propertyname, Bind all Stuff
		CurrentTimeline->AddInterpVector(VectorCurve, onTimelineCallbackVector,
			FName("MovementVec"));
		CurrentTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		// Timeline Settings
		CurrentTimeline->SetLooping(TimelineIsLooping);
		//false = it moves as the global Time Dileation
		CurrentTimeline->SetIgnoreTimeDilation(IgnoreTimeDilation);
	}
}
