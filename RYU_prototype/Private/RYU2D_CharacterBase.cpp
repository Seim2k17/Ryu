// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/RYU2D_AnimationComponent.h"
#include "Components/RYU2D_MovementComponent.h"
#include "RYUENUM_LedgeSideEntered.h"
#include "RYUClimbingActor.h"
#include "Components/BoxComponent.h"



ARYU2D_CharacterBase::ARYU2D_CharacterBase()
{

}

ARYU2D_CharacterBase::ARYU2D_CharacterBase(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<URYU2D_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	Animation2DComponent = CreateDefaultSubobject<URYU2D_AnimationComponent>(TEXT("P2D_Animations"));
	
	SphereTracer = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTracer"));
	SphereTracer->SetupAttachment(RootComponent);

	SphereTracer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereTracer->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereTracer->SetRelativeLocation(FVector(60, 0, 0));
	SphereTracer->SetSphereRadius(60);

	RYUMovement = ERYUMovementMode::STAND;
	
	ESideEntered = ERYULedgeSideEntered::NONE;

	CanClimbUpTagName = "CanClimbUp";
	CanClimbDownTagName = "CanClimbDown";

	LeftLedgePosiTagName = "Left";
	RightLedgePosiTagName = "Right";

	bSphereTracerOverlap = false;

	bJumpJustStarted = false;

	TreshholdYWalkRun = 220.0f;

}

void ARYU2D_CharacterBase::OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	OnSphereTracerCheckOverlap(OtherActor, OtherComp);
}

void ARYU2D_CharacterBase::OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSphereTracerOverlap = false;
	bLedgeHeightInRange = false;
	SphereOverlappedActor = nullptr;
	UE_LOG(LogTemp, Log, TEXT("SpherTracer Overlap Out"));
	if ((RYUClimbingMode != ERYUClimbingMode::CANCLIMBDOWNLEDGE) &&
		(RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN) &&
		(RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPLEDGE) &&
		(RYUClimbingMode != ERYUClimbingMode::CLIMBDOWNLEDGE) &&
		(RYUClimbingMode != ERYUClimbingMode::CLIMBUPLEDGE) &&
		(RYUMovement != ERYUMovementMode::CLIMB))
	{
		UE_LOG(LogTemp, Log, TEXT("SphereTracer: Mode: Walk Activated"));
		RYUMovement = ERYUMovementMode::WALK;
	}
}

void ARYU2D_CharacterBase::OnSphereTracerCheckOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	ARYUClimbingActor* IgnoredActor_1 = Cast<ARYUClimbingActor>(OtherActor);
	if (OtherActor != nullptr) UE_LOG(LogTemp, Log, TEXT("SphereTracer Overlap In with: %s"), *OtherActor->GetName());

	if ((OtherActor != nullptr) &&
		//(!ClimbComponentOverlap) &&
		(!IgnoredActor_1))
	{
		//double Set when calling from OnHandleCapsuleEndOverlap, But WHO CARES ?
		SphereOverlappedActor = OtherActor;
		SphereOverlappedComponent = OtherComp;

		bSphereTracerOverlap = true;
		if ((RYUClimbingMode != ERYUClimbingMode::CANCLIMBDOWNLEDGE) &&
			(RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN) &&
			(RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPLEDGE) &&
			(RYUClimbingMode != ERYUClimbingMode::CLIMBDOWNLEDGE) &&
			(RYUClimbingMode != ERYUClimbingMode::CLIMBUPLEDGE) &&
			(RYUMovement != ERYUMovementMode::CLIMB))

		{
			UE_LOG(LogTemp, Log, TEXT("SphereTracer: Mode: CanTraceActivated"));
			//RYUMovement = ERYUMovementMode::CANTRACELEDGE;
			RYUMovement = ERYUMovementMode::CANGRABLEDGE;
		}

	}
}

UBoxComponent* ARYU2D_CharacterBase::GetOverlappedClimbingComponent(FName UpOrDown, FName LeftOrRight)
{
	UBoxComponent* OverlappedClimbingComp = nullptr;

	for (int i = 0; i < CapsuleOverlappedComponents.Num(); i++)
	{
		UBoxComponent* OvCC = Cast<UBoxComponent>(CapsuleOverlappedComponents[i]);
		if (OvCC)
		{
			//it´s a Boxtrigger for sure
			//we assume and due gamedesign we pretend that we ONLY can overlapp ONE component from ONE actor but TWO actors at a time
			if ((OvCC->ComponentTags[0] == UpOrDown) &&
				(OvCC->ComponentTags[1] == LeftOrRight))
			{
				OverlappedClimbingComp = OvCC;
				break;
			}

		}
	}

	return OverlappedClimbingComp;
}


void ARYU2D_CharacterBase::OnHandleCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("CapslOvlp in: %s "), *OtherComp->GetName());

	//ToDo: need to handle other Triggerlements than ClimbingTriggers ???, for now and easyness we only do ClimbingTriggers

	ARYUClimbingActor* ARY = Cast<ARYUClimbingActor>(OtherActor);
	if (ARY)
	{


		//we work only with the components not with the actor itself
		if (((OtherComp->ComponentTags[0] == CanClimbDownTagName) && ((ARY->bLeftDownTriggerIsActive) || (ARY->bRightDownTriggerIsActive))) &&
			((OtherComp->ComponentTags[0] == CanClimbUpTagName) && ((ARY->bLeftUpTriggerIsActive) || (ARY->bRightUpTriggerIsActive))))
		{
			CapsuleOverlappedComponents.Add(OtherComp);
		}


		UE_LOG(LogTemp, Log, TEXT("CapslOvlpArray with: %d Actors"), CapsuleOverlappedComponents.Num());

		//easy check but u need to bee careful !!!
		if (CapsuleOverlappedComponents.Num() > 1)
		{
			RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;
			return;
		}

		if (RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN)
		{
			if (OtherComp->ComponentTags[0] == CanClimbDownTagName)
			{
				UE_LOG(LogTemp, Log, TEXT("CanClimbDown TAG: Overlap In"));
				RYUClimbingMode = ERYUClimbingMode::CANCLIMBDOWNLEDGE;
			}

			if (OtherComp->ComponentTags[0] == CanClimbUpTagName)
			{
				UE_LOG(LogTemp, Log, TEXT("CanClimbUp TAG: Overlap In"));
				RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPLEDGE;
			}


			//if UpAND DOwn the position needs to be set in Climb !
			if (OtherComp->ComponentTags[1] == LeftLedgePosiTagName)
			{
				SetLedgeHangPosition(ARY->LeftHangPosition->GetComponentLocation(), LeftLedgePosiTagName);
				UE_LOG(LogTemp, Log, TEXT("TAG: %s"), *LeftLedgePosiTagName.ToString());
			}

			if (OtherComp->ComponentTags[1] == RightLedgePosiTagName)
			{
				SetLedgeHangPosition(ARY->RightHangPosition->GetComponentLocation(), RightLedgePosiTagName);
				UE_LOG(LogTemp, Log, TEXT("TAG: %s"), *RightLedgePosiTagName.ToString());
			}
		}
	}
}


void ARYU2D_CharacterBase::OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("CapslOvlp out: %s"), *OtherComp->GetName());
	RYUClimbingMode = ERYUClimbingMode::NONE;
	RYUMovement = ERYUMovementMode::STAND;
	SetLedgeHangPosition(FVector::ZeroVector, "none");

	CapsuleOverlappedComponents.RemoveSingle(OtherComp);

	//Check if an actor is is collided with the SphereTracer
	OnSphereTracerCheckOverlap(SphereOverlappedActor, SphereOverlappedComponent);
}

void ARYU2D_CharacterBase::SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide)
{
	LedgeHangPosition = LedgeTargetPoint;
	LedgeHangPosition.Z = LedgeHangPosition.Z + 115.0f;

	//	LeftPositionTag = "Left";
	//RightPositionTag = "Right";

	if (LedgeSide == "Left")
	{
		UE_LOG(LogTemp, Log, TEXT("Left Side Ledge."));
		ESideEntered = ERYULedgeSideEntered::LeftSide;
	}
	else
	{
		if (LedgeSide == "Right")
		{
			UE_LOG(LogTemp, Log, TEXT("Right Side Ledge."));
			ESideEntered = ERYULedgeSideEntered::RightSide;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("no Side Ledge."));
			ESideEntered = ERYULedgeSideEntered::NONE;
		}
	}
}


FVector ARYU2D_CharacterBase::GetLedgeHangPosition()
{
	return LedgeHangPosition;
}


ERYULedgeSideEntered ARYU2D_CharacterBase::GetLedgeSideEntered()
{
	return ESideEntered;
}


void ARYU2D_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARYU2D_CharacterBase::OnHandleCapsuleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ARYU2D_CharacterBase::OnHandleCapsuleEndOverlap);
}

/** General Movement Stuff*/

void ARYU2D_CharacterBase::MoveRight(float Val)
{
	if (FMath::Abs(GetCharacterMovement()->Velocity.Y) > 0)
	{
		if (FMath::Abs(GetCharacterMovement()->Velocity.Y) > TreshholdYWalkRun)
		{
			RYUMovement = ERYUMovementMode::RUN;
		}
		else
		{
			RYUMovement = ERYUMovementMode::WALK;
		}
	}
	else
	{
		RYUMovement = ERYUMovementMode::STAND;
	}
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Val);
}

void ARYU2D_CharacterBase::Jump()
{

}

void ARYU2D_CharacterBase::StopJumping()
{

}


