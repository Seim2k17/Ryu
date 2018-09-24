// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/RYU2D_MovementComponent.h"
#include "RYUENUM_LedgeSideEntered.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"
#include "Components/BoxComponent.h"
#include "Math/Vector.h"



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

	SphereTracer = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTracer"));
	SphereTracer->SetupAttachment(RootComponent);

	SphereTracer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereTracer->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereTracer->SetRelativeLocation(FVector(60, 0, 0));
	SphereTracer->SetSphereRadius(60);

	PlayerMovement = EPlayerMovement::STAND;
	
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
	//OnSphereTracerCheckOverlap(OtherActor, OtherComp);
}

void ARYU2D_CharacterBase::OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
// 	bSphereTracerOverlap = false;
// 	bLedgeHeightInRange = false;
// 	SphereOverlappedActor = nullptr;
// 	UE_LOG(LogTemp, Log, TEXT("OnSphereTracerHandleEndOverlap(): SpherTracer Overlap Out"));
// 	if ((RYUClimbingMode != ERYUClimbingMode::CANCLIMBDOWNLEDGE) &&
// 		(RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN) &&
// 		(RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPLEDGE) &&
// 		(RYUClimbingMode != ERYUClimbingMode::CLIMBDOWNLEDGE) &&
// 		(RYUClimbingMode != ERYUClimbingMode::CLIMBUPLEDGE) &&
// 		(PlayerMovement != EPlayerMovement::CLIMBING))
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("OnSphereTracerHandleEndOverlap(): SphereTracer->Mode->Walk Activated"));
// 		PlayerMovement = EPlayerMovement::WALK;
// 	}
}

void ARYU2D_CharacterBase::OnSphereTracerCheckOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	ARYUClimbingActor* IgnoredActor_1 = Cast<ARYUClimbingActor>(OtherActor);
	if (OtherActor != nullptr) UE_LOG(LogTemp, Log, TEXT("OnSphereTracerCheckOverlap(): SphereTracer Overlap In with: %s"), *OtherActor->GetName());

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
			(PlayerMovement != EPlayerMovement::CLIMBING))

		{
			UE_LOG(LogTemp, Log, TEXT("OnSphereTracerCheckOverlap(): SphereTracer->Mode->CanTraceActivated"));
			//PlayerMovement = EPlayerMovementMode::CANTRACELEDGE;
			//PlayerMovement = EPlayerMovement::CANGRABLEDGE;
		}

	}
}

UBoxComponent* ARYU2D_CharacterBase::GetOverlappedClimbingComponent(FName UpOrDown, FName LeftOrRight)
{

	UE_LOG(LogTemp, Log, TEXT("GetOverlappedClimbComp(): %s , %s"),*UpOrDown.ToString(),*LeftOrRight.ToString());

	UBoxComponent* OverlappedClimbingComp = nullptr;
	FName ClimbCheck;

	if (RYUClimbingMode == ERYUClimbingMode::CANCLIMBDOWNLEDGE)
	{
		ClimbCheck = CanClimbDownTagName;
	}
	if (RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPLEDGE)
	{
		ClimbCheck = CanClimbUpTagName;
	}

	UE_LOG(LogTemp, Log, TEXT("GetOverlappedClimbComp(): ClimbCheck: %s"), *ClimbCheck.ToString());

	for (int i = 0; i < CapsuleOverlappedComponents.Num(); i++)
	{
		UBoxComponent* OvCC = Cast<UBoxComponent>(CapsuleOverlappedComponents[i]);
		if (OvCC)
		{
			//it´s a Boxtrigger for sure
			//we assume and due gamedesign we pretend that we ONLY can overlapp ONE component from ONE actor but TWO actors at a time
			if ((OvCC->ComponentTags[0] == UpOrDown) && (ClimbCheck == UpOrDown) &&
				(OvCC->ComponentTags[1] == LeftOrRight))
			{
				OverlappedClimbingComp = OvCC;
				break;
			}

		}
	}

	return OverlappedClimbingComp;
}


UBoxComponent* ARYU2D_CharacterBase::GetLowerOverlappedClimbingComponent(FName LowerTrigger)
{
	UE_LOG(LogTemp, Log, TEXT("GetLowerOverlappedClimbingComponent(): %s"), *LowerTrigger.ToString());
	UBoxComponent* LowerBox = Cast<UBoxComponent>(CapsuleOverlappedComponents[0]);

	if (CapsuleOverlappedComponents.Num() == 2)
	{
		FVector Posi1 = CapsuleOverlappedComponents[0]->GetOwner()->GetActorLocation();
		FVector Posi2 = CapsuleOverlappedComponents[1]->GetOwner()->GetActorLocation();
		//we dirty assume that we only stay inside 2 Triggers maximu
		if (Posi1.Z < Posi2.Z)
		{
			if (Cast<UBoxComponent>(CapsuleOverlappedComponents[0]))
			{
				LowerBox = Cast<UBoxComponent>(CapsuleOverlappedComponents[0]);
				UE_LOG(LogTemp, Log, TEXT("GetLowerOverlappedClimbingComponent(): lower Component is %s"), *CapsuleOverlappedActors[0]->GetName());
			}
			
		}
		else
		{

			if (Cast<UBoxComponent>(CapsuleOverlappedComponents[1]))
			{
				LowerBox = Cast<UBoxComponent>(CapsuleOverlappedComponents[1]);
				UE_LOG(LogTemp, Log, TEXT("GetLowerOverlappedClimbingComponent(): lower Component is %s"), *CapsuleOverlappedActors[1]->GetName());
			}
		}
	}
	
	
	return LowerBox;
}

void ARYU2D_CharacterBase::OnHandleCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleBeginOverlap(): CapslOvlp out from %s"), *OtherComp->GetName());
}


void ARYU2D_CharacterBase::OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleEndOverlap(): CapslOvlp out from %s"), *OtherComp->GetName());

}


void ARYU2D_CharacterBase::CheckOverlappingActors()
{
	UE_LOG(LogTemp, Log, TEXT("CheckOverlappingActors(): "));

	GetOverlappingActors(CapsuleOverlappedActors, ClimbableActorClass);

	if (CapsuleOverlappedActors.Num() > 0)
	{
		CheckOverlappingComponents();
	}
	else
	{
		PlayerMovement = EPlayerMovement::STAND;
		RYUClimbingMode = ERYUClimbingMode::NONE;
	}
	
	
}


void ARYU2D_CharacterBase::CheckOverlappingComponents()
{
	//I think we rewrite this part ! -> think first then code
	//@ToDo:

	for (int i = 0; i < CapsuleOverlappedActors.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): %s"), *CapsuleOverlappedActors[i]->GetName());
	}

}


void ARYU2D_CharacterBase::SetClimbingPositions(UBoxComponent* ClimbTrigger)
{

	ARYUClimbingActor* ARY = Cast<ARYUClimbingActor>(ClimbTrigger->GetOwner());

	if (ARY)
	{
		//if UpAND DOwn the position needs to be set in Climb !
		if (CurrentLedgePosiTagName == LeftLedgePosiTagName)
		{
			//@Relict?
			SetLedgeHangPosition(ARY->LeftHangPosition->GetComponentLocation(), LeftLedgePosiTagName);
			//NewPositioning

			if (RYUClimbingMode == ERYUClimbingMode::CLIMBDOWNLEDGE)
			{
				ClimbStandDownPosition = ARY->DownRightStandPosition->GetComponentLocation();
				ClimbStandUpPosition = ARY->UpLeftStandPosition->GetComponentLocation();
			}
			if (RYUClimbingMode == ERYUClimbingMode::CLIMBUPLEDGE)
			{
				ClimbStandDownPosition = ARY->DownLeftStandPosition->GetComponentLocation();
				ClimbStandUpPosition = ARY->UpRightStandPosition->GetComponentLocation();
			}

		}

		if (CurrentLedgePosiTagName == RightLedgePosiTagName)
		{
			//@Relict?
			SetLedgeHangPosition(ARY->RightHangPosition->GetComponentLocation(), RightLedgePosiTagName);
			//NewPositioning

			if (RYUClimbingMode == ERYUClimbingMode::CLIMBDOWNLEDGE)
			{
				ClimbStandDownPosition = ARY->DownLeftStandPosition->GetComponentLocation();
				ClimbStandUpPosition = ARY->UpRightStandPosition->GetComponentLocation();
			}
			if (RYUClimbingMode == ERYUClimbingMode::CLIMBUPLEDGE)
			{
				ClimbStandDownPosition = ARY->DownRightStandPosition->GetComponentLocation();
				ClimbStandUpPosition = ARY->UpLeftStandPosition->GetComponentLocation();
			}
		}


		UE_LOG(LogTemp, Log, TEXT("SetClimbingPositions(): ClimbStandDownPosi is %s"), *ClimbStandDownPosition.ToString());
		UE_LOG(LogTemp, Log, TEXT("SetClimbingPositions(): ClimbStandUp is %s"), *ClimbStandUpPosition.ToString());
	}
	
}

void ARYU2D_CharacterBase::FlipCharacter()
{

	//float TravelDirection = currV.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		//if (TravelDirection < 0.0f)
		if (bLookRight)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
			CameraBoom->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
		}
		//else if (TravelDirection > 0.0f)
		else
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
		}
	}

	bLookRight = !bLookRight;
	UE_LOG(LogTemp, Log, TEXT("FlipCharacter(): lookRight = %s"), bLookRight ? TEXT("true") : TEXT("false"));
	//coa vs reset ?
	CheckOverlappingActors();
}

void ARYU2D_CharacterBase::SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide)
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
			PlayerMovement = EPlayerMovement::RUN;
		}
		else
		{
			PlayerMovement = EPlayerMovement::WALK;
		}
	}
	else
	{
		PlayerMovement = EPlayerMovement::STAND;
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


