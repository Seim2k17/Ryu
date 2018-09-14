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
	if (PlayerMovement != EPlayerMovement::CLIMBING)
	{

		UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleBeginOverlap(): CapsuleOverlap in from %s "), *OtherActor->GetName());

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

				CapsuleOverlappedComponents.AddUnique(OtherComp);
				//UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleBeginOverlap(): CapslOvlpArray[%d] is %s"), i, *CapsuleOverlappedComponents[i]->GetName());
				i++;
			}


			//UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleBeginOverlap(): CapslOvlpArray with: %d Actors"), CapsuleOverlappedComponents.Num());

			CheckOverlappingComponents();
		}
	}
	
}


void ARYU2D_CharacterBase::OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleEndOverlap(): CapslOvlp out from %s"), *OtherComp->GetName());
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
	//Here We only check for ClimbingActors!
	GetCapsuleComponent()->GetOverlappingActors(CapsuleOverlappedActors, ARYUClimbingActor::StaticClass());
	//UE_LOG(LogTemp, Log, TEXT("Call CheckOverlappingActors"));
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
			//Check if there is an ledge overlapping
			for (int i = 0; i < CapsuleOverlappedActors.Num(); i++)
			{
				UE_LOG(LogTemp, Log, TEXT("CheckOverlappingActors(): Capsule[%d]: %s"),i,*CapsuleOverlappedActors[i]->GetName());
				if (RYUClimbingMode == ERYUClimbingMode::LETGOLEDGE)
				{
					GetOverlappingComponents(CapsuleOverlappedComponents);
					for (int i = CapsuleOverlappedComponents.Num(); i > 0; i--)
					{
						UBoxComponent* CapsEl = Cast<UBoxComponent>(CapsuleOverlappedComponents[i-1]);
						if (!CapsEl)
						{
							CapsuleOverlappedComponents.RemoveAt(i-1);
						}
					}
					//Recheck CapsuleEnterstuff
					
					//we work only with the components not with the actor itself
// 					if ((OtherComp->ComponentTags[0] == CanClimbDownTagName) || (OtherComp->ComponentTags[0] == CanClimbUpTagName))
// 					{
// 
// 						CapsuleOverlappedComponents.AddUnique(OtherComp);
// 						//UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleBeginOverlap(): CapslOvlpArray[%d] is %s"), i, *CapsuleOverlappedComponents[i]->GetName());
// 						i++;
// 					}

				}
			}
			//Obviously there are 1+ Climbingtrigger
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
	//I think we rewrite this part ! -> think first then code
	//@ToDo: here is something wrong: at start it adds something like RenderComponent / CollisionCylinder from other Owner than ClimbingActors (like POP_BG_Lv1_2 )
	//changed some stuff to checkoverlapped actors !!!!
	UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): %d"), CapsuleOverlappedComponents.Num());
	if (CapsuleOverlappedComponents.Num() >= 1)
	{
		for (int k = 0; k < CapsuleOverlappedComponents.Num(); k++)
		{
			UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): Component is %s, Owner: %s"), *CapsuleOverlappedComponents[k]->GetName(), *CapsuleOverlappedComponents[k]->GetOwner()->GetName());
		}
	}
	
	
	
	int j = 0;
	for (j; j < CapsuleOverlappedActors.Num(); j++)
	{
		UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): Actor is %s"), *CapsuleOverlappedActors[j]->GetName());
	}
	//@ToDo: look here for more than1 TriggerOverlappCrap, first need: breakfast
	if (PlayerMovement != EPlayerMovement::CLIMBING)
	{
		if (!bDoThingsOnce)
		{
			//bDoThingsOnce = true;
			//easy check but u need to bee careful !!!
			if (CapsuleOverlappedActors.Num() > 1)
			{
				RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;
				return;
			}
			else if (CapsuleOverlappedActors.Num() == 1)
			{
				ARYUClimbingActor* ARY = Cast<ARYUClimbingActor>(CapsuleOverlappedActors[0]);
				if (ARY)
				{
					if (RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN)
					{
						//we assume that we only overlap ONE Component at a time from one Climbing Actor
						
						if (CapsuleOverlappedComponents.Num() == 0)
						{
							CapsuleOverlappedActors[0]->GetOverlappingComponents(CapsuleOverlappedComponents);
							//need to delete the comps which are not boxcomponents

							UE_LOG(LogTemp, Log, TEXT("Capsule.Num() war 0"));
						}
						
						
						if (CapsuleOverlappedComponents.Num() == 1)
						{
							//@ToDo: CRASH: Check why Char not enters this branch sometimes after LetGoLedgeFinished is Fired (see pop Level1)
							UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents"));
							if (CapsuleOverlappedComponents[0]->ComponentTags[0] == CanClimbDownTagName)
							{
								UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): CanClimbDown TAG is Overlap In"));
								RYUClimbingMode = ERYUClimbingMode::CANCLIMBDOWNLEDGE;
								CurrentClimbTagName = CanClimbDownTagName;
							}

							if (CapsuleOverlappedComponents[0]->ComponentTags[0] == CanClimbUpTagName)
							{
								UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): CanClimbUp TAG is Overlap In"));
								RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPLEDGE;
								CurrentClimbTagName = CanClimbUpTagName;
							}

							if (CapsuleOverlappedComponents[0]->ComponentTags[1] == LeftLedgePosiTagName)
							{
								CurrentLedgePosiTagName = LeftLedgePosiTagName;
							}

							if (CapsuleOverlappedComponents[0]->ComponentTags[1] == RightLedgePosiTagName)
							{
								CurrentLedgePosiTagName = RightLedgePosiTagName;
							}


							UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): Trigger-TAG is %s"), *CurrentLedgePosiTagName.ToString());
						}
					}
				}
			}
		}
		
		
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


