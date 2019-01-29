// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/RYU2D_MovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/Vector.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"

ARYU2D_CharacterBase::ARYU2D_CharacterBase()
{
}

ARYU2D_CharacterBase::ARYU2D_CharacterBase(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<URYU2D_MovementComponent>(
        ACharacter::CharacterMovementComponentName))
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

    //Constant
    CanClimbUpTagName = "CanClimbUp";
    CanClimbDownTagName = "CanClimbDown";
    CanClimbUpDownName = "CanClimbUpAndDown";

    LeftLedgePosiTagName = "Left";
    RightLedgePosiTagName = "Right";

    bSphereTracerOverlap = false;

    bJumpJustStarted = false;

    TreshholdYWalkRun = 220.0f;
}

void ARYU2D_CharacterBase::OnSphereTracerHandleBeginOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //OnSphereTracerCheckOverlap(OtherActor, OtherComp);
}

void ARYU2D_CharacterBase::OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                          AActor* OtherActor,
                                                          UPrimitiveComponent* OtherComp,
                                                          int32 OtherBodyIndex)
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

void ARYU2D_CharacterBase::OnSphereTracerCheckOverlap(AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp)
{
    ARYUClimbingActor* IgnoredActor_1 = Cast<ARYUClimbingActor>(OtherActor);
    if (OtherActor != nullptr)
        UE_LOG(LogTemp, Log, TEXT("OnSphereTracerCheckOverlap(): SphereTracer Overlap In with: %s"),
               *OtherActor->GetName());

    if ((OtherActor != nullptr) &&
        //(!ClimbComponentOverlap) &&
        (!IgnoredActor_1))
    {
        //double Set when calling from OnHandleCapsuleEndOverlap, But WHO CARES ?
        SphereOverlappedActor = OtherActor;
        SphereOverlappedComponent = OtherComp;

        bSphereTracerOverlap = true;
        if ((RYUClimbingMode != ERYUClimbingMode::CANCLIMBDOWNLEDGE)
            && (RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN)
            && (RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPLEDGE)
            && (RYUClimbingMode != ERYUClimbingMode::CLIMBDOWNLEDGE)
            && (RYUClimbingMode != ERYUClimbingMode::CLIMBUPLEDGE)
            && (PlayerMovement != EPlayerMovement::CLIMBING))

        {
            UE_LOG(LogTemp, Log,
                   TEXT("OnSphereTracerCheckOverlap(): SphereTracer->Mode->CanTraceActivated"));
            //PlayerMovement = EPlayerMovementMode::CANTRACELEDGE;
            //PlayerMovement = EPlayerMovement::CANGRABLEDGE;
        }
    }
}

void ARYU2D_CharacterBase::OnHandleCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                       AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp,
                                                       int32 OtherBodyIndex, bool bFromSweep,
                                                       const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleBeginOverlap(): CapslOvlp in from %s"),
           *OtherComp->GetName());
}

void ARYU2D_CharacterBase::OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                     AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp,
                                                     int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleEndOverlap(): CapslOvlp out from %s"),
           *OtherComp->GetName());
}

void ARYU2D_CharacterBase::CheckOverlappingActors()
{
    UE_LOG(LogTemp, Log, TEXT("CheckOverlappingActors(): "));

    GetOverlappingActors(CapsuleOverlappedActors, ClimbableActorClass);

    for (int i = 0; i < CapsuleOverlappedActors.Num(); i++)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] : %s"), *FString::FromInt(i),
               *CapsuleOverlappedActors[i]->GetName());
    }

    if (CapsuleOverlappedActors.Num() > 0)
    {
        CheckOverlappingComponents();
    }
    else
    {
        PlayerMovement = EPlayerMovement::STAND;
        RYUClimbingMode = ERYUClimbingMode::NONE;
        CurrentLedgePosiTagName = "";
        CurrentClimbTagName = "";
    }
}

void ARYU2D_CharacterBase::CheckOverlappingComponents()
{
    //I think we rewrite this part ! -> think first then code
    //TODO:

    //BY DESIGN: max. overlapped climbing Triggers == 2
    //check if overlapped comp right or left

    //check if overlapped comp up or/and down

    int countTrigger = CapsuleOverlappedActors.Num();
    UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): %s"), *FString::FromInt(countTrigger));

    if (countTrigger > 2)
    {
        UE_LOG(LogTemp, Warning,
               TEXT("CheckOverlappingComponents(): To many overlapping ClimbingTrigger (%s). "
                    "Please realign ClimbingAssets!"),
               *FString::FromInt(countTrigger));
        UE_LOG(LogTemp, Warning, TEXT("CheckOverlappingComponents(): Trigger are:"));
        for (int i = 0; i < countTrigger; i++)
        {
            UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): %s"),
                   *CapsuleOverlappedActors[i]->GetName());
        }
    }
    else
    {
        PlayerMovement = EPlayerMovement::CANGRABLEDGE;

        if (countTrigger == 1)
        {
            //save all Overlapping Components in a arry, before that we make sure that the array is empty
            //CapsuleOverlappedComponents.Empty(); wird in

            //we know that the TriggerComponents we search for, are UBoxComponents and we do not need any other
            GetOverlappingBoxComponents();

            //CapsuleOverlappedActors[0]->GetOverlappingComponents(CapsuleOverlappedComponents);

            //ERYULedgePosition2D LedgePosition = ERYULedgePosition2D::NONE;
            //ERYULedgeSideEntered LedgeSide = ERYULedgeSideEntered::NONE;

            ERYULedgePosition2D LedgePosition = GetLedgePosition();
            ERYULedgeSideEntered LedgeSide = GetLedgeSide(0);

            /* fast check what kind of trigger*/
            OutputCapsuleOverlappedComponents();
            //UBoxComponent* OverlapClimbComp = GetOverlappedClimbingComponent();

            UE_LOG(LogTemp, Log,
                   TEXT("CheckOverlappingComponents(): Yes it´s %s with the Trigger at ... "),
                   *CapsuleOverlappedActors[0]->GetName());

            if (LedgePosition == ERYULedgePosition2D::PosiDown)
            {
                RYUClimbingMode = ERYUClimbingMode::CANCLIMBDOWNLEDGE;
            }

            if (LedgePosition == ERYULedgePosition2D::PosiUp)
            {
                RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPLEDGE;
            }

            if (LedgeSide == ERYULedgeSideEntered::LeftSide)
            {
                UE_LOG(LogTemp, Log, TEXT("... the left Side of the Actor.."));
            }

            if (LedgeSide == ERYULedgeSideEntered::RightSide)
            {
                UE_LOG(LogTemp, Log, TEXT("... the right Side of the Actor."));
            }
        }

        if (countTrigger == 2)
        {
            //Check at Input: we know we can Move Up OR Down
            //check if overlapped comp up or/and down
            //check if overlapped comp right or left

            RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;
            ERYULedgePosition2D LedgePosition = GetLedgePosition();
        }
    }
}

ERYULedgeSideEntered ARYU2D_CharacterBase::GetLedgeSide(int posi)
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

/*
param posi - Position in the OverlappedComponentArray to receive the correct TriggerBox
*/
ERYULedgePosition2D ARYU2D_CharacterBase::GetLedgePosition()
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

void ARYU2D_CharacterBase::OutputCapsuleOverlappedComponents()
{
    for (int i = 0; i < CapsuleOverlappedComponents.Num(); i++)
    {
        UPrimitiveComponent* el = CapsuleOverlappedComponents[i];
        UE_LOG(LogTemp, Log, TEXT("OutputCapsuleOverlapArray[%s]: %s Posi: %s Owner: %s"),
               *FString::FromInt(i), *el->GetName(), *el->GetOwner()->GetActorLocation().ToString(),
               *el->GetOwner()->GetName());
    }
}

void ARYU2D_CharacterBase::SetPlayerMovement(EPlayerMovement PlayerStateToSet)
{
    PlayerMovement = PlayerStateToSet;
}

EPlayerMovement ARYU2D_CharacterBase::GetPlayerMovement()
{
    return PlayerMovement;
}

void ARYU2D_CharacterBase::SetClimbingMode(ERYUClimbingMode ClimbingModeToSet)
{
    RYUClimbingMode = ClimbingModeToSet;
}

ERYUClimbingMode ARYU2D_CharacterBase::GetClimbingMode()
{
    return RYUClimbingMode;
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
    UE_LOG(LogTemp, Log, TEXT("FlipCharacter(): lookRight = %s"),
           bLookRight ? TEXT("true") : TEXT("false"));
    //coa vs reset ?
    CheckOverlappingActors();
}

void ARYU2D_CharacterBase::GetOverlappingBoxComponents()
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

void ARYU2D_CharacterBase::ToggleEnterLedgeSide()
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

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(
        this, &ARYU2D_CharacterBase::OnHandleCapsuleBeginOverlap);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(
        this, &ARYU2D_CharacterBase::OnHandleCapsuleEndOverlap);
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
