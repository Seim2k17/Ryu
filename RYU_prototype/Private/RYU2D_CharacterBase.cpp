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
		(PlayerMovement != EPlayerMovement::CLIMBING))
	{
		UE_LOG(LogTemp, Log, TEXT("SphereTracer: Mode: Walk Activated"));
		PlayerMovement = EPlayerMovement::WALK;
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
			(PlayerMovement != EPlayerMovement::CLIMBING))

		{
			UE_LOG(LogTemp, Log, TEXT("SphereTracer: Mode: CanTraceActivated"));
			//PlayerMovement = EPlayerMovementMode::CANTRACELEDGE;
			PlayerMovement = EPlayerMovement::CANGRABLEDGE;
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
	if (PlayerMovement != EPlayerMovement::CLIMBING)
	{
		UE_LOG(LogTemp, Log, TEXT("CapslOvlp in: %s "), *OtherComp->GetName());

		//ToDo: need to handle other Triggerlements than ClimbingTriggers ???, for now and easyness we only do ClimbingTriggers

		ARYUClimbingActor* ARY = Cast<ARYUClimbingActor>(OtherActor);
		if (ARY)
		{
			//if we run into the ClimbingActors
			if ((PlayerMovement != EPlayerMovement::RUN) &&
				(PlayerMovement != EPlayerMovement::ENDRUN))
			{
				PlayerMovement = EPlayerMovement::CANGRABLEDGE;
			}

			int i = 0;
			//we work only with the components not with the actor itself
			if ((OtherComp->ComponentTags[0] == CanClimbDownTagName) || (OtherComp->ComponentTags[0] == CanClimbUpTagName))
			{

				CapsuleOverlappedComponents.Add(OtherComp);
				UE_LOG(LogTemp, Log, TEXT("CapslOvlpArray[%d] : %s"), i, *CapsuleOverlappedComponents[i]->GetName());
				i++;
			}


			UE_LOG(LogTemp, Log, TEXT("CapslOvlpArray with: %d Actors"), CapsuleOverlappedComponents.Num());

			CheckOverlappingComponents();


		}
	}
	
}


void ARYU2D_CharacterBase::OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("CapslOvlp out: %s"), *OtherComp->GetName());
	RYUClimbingMode = ERYUClimbingMode::NONE;
	//does not function for 2D Stuff
	//PlayerMovement = EPlayerMovement::STAND;
	SetLedgeHangPosition(FVector::ZeroVector, "none");

	CapsuleOverlappedComponents.RemoveSingle(OtherComp);

	//Check if an actor is is collided with the SphereTracer
	OnSphereTracerCheckOverlap(SphereOverlappedActor, SphereOverlappedComponent);
}


void ARYU2D_CharacterBase::CheckOverlappingActors()
{

	GetOverlappingActors(CapsuleOverlappedActors);
	UE_LOG(LogTemp, Log, TEXT("Call CheckOverlappingActors"));
	//if there are CapsuleOverlappingActors set the appr. Movement / ClimbingMode (important for EndFinished-Notifier in the ABP
	//if (CapsuleOverlappedComponents.Num() > 0)
	if (CapsuleOverlappedActors.Num() > 0)
	{
		if (PlayerMovement == EPlayerMovement::STARTTURN)
		{
			PlayerMovement = EPlayerMovement::ENDTURN;
		}
		else
		{
			PlayerMovement = EPlayerMovement::CANGRABLEDGE;
		}
		
		CheckOverlappingComponents();
	}
	else
	{
		if (PlayerMovement == EPlayerMovement::STARTTURN)
		{
			PlayerMovement = EPlayerMovement::ENDTURN;
		}
		else
		{
			PlayerMovement = EPlayerMovement::STAND;
		}
	}
}


void ARYU2D_CharacterBase::CheckOverlappingComponents()
{
	UE_LOG(LogTemp, Log, TEXT("Call CheckOverlappingComponents: %d"), CapsuleOverlappedComponents.Num());
	if (PlayerMovement != EPlayerMovement::CLIMBING)
	{
		//easy check but u need to bee careful !!!
		if (CapsuleOverlappedComponents.Num() > 1)
		{
			RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;
			return;
		}
		else if (CapsuleOverlappedComponents.Num() == 1)
		{
			ARYUClimbingActor* ARY = Cast<ARYUClimbingActor>(CapsuleOverlappedComponents[0]->GetOwner());
			if (ARY)
			{
				if (RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN)
				{
					if (CapsuleOverlappedComponents[0]->ComponentTags[0] == CanClimbDownTagName)
					{
						UE_LOG(LogTemp, Log, TEXT("CanClimbDown TAG: Overlap In"));
						RYUClimbingMode = ERYUClimbingMode::CANCLIMBDOWNLEDGE;
					}

					if (CapsuleOverlappedComponents[0]->ComponentTags[0] == CanClimbUpTagName)
					{
						UE_LOG(LogTemp, Log, TEXT("CanClimbUp TAG: Overlap In"));
						RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPLEDGE;
					}


					//if UpAND DOwn the position needs to be set in Climb !
					if (CapsuleOverlappedComponents[0]->ComponentTags[1] == LeftLedgePosiTagName)
					{
						//@Relict?
						SetLedgeHangPosition(ARY->LeftHangPosition->GetComponentLocation(), LeftLedgePosiTagName);
						//NewPositioning
						ClimbStandDownPosition = ARY->DownLeftStandPosition->GetComponentLocation();
						ClimbStandUpPosition = ARY->UpLeftStandPosition->GetComponentLocation();
						UE_LOG(LogTemp, Log, TEXT("TAG: %s"), *LeftLedgePosiTagName.ToString());
					}

					if (CapsuleOverlappedComponents[0]->ComponentTags[1] == RightLedgePosiTagName)
					{
						//@Relict?
						SetLedgeHangPosition(ARY->RightHangPosition->GetComponentLocation(), RightLedgePosiTagName);
						//NewPositioning
						ClimbStandDownPosition = ARY->DownRightStandPosition->GetComponentLocation();
						ClimbStandUpPosition = ARY->UpRightStandPosition->GetComponentLocation();
						UE_LOG(LogTemp, Log, TEXT("TAG: %s"), *RightLedgePosiTagName.ToString());
					}

					UE_LOG(LogTemp, Log, TEXT("ClimbStandDown: %s"), *ClimbStandDownPosition.ToString());
					UE_LOG(LogTemp, Log, TEXT("ClimbStandUp: %s"), *ClimbStandUpPosition.ToString());
				}
			}
		}
		
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


