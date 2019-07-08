// Fill out your copyright notice in the Description page of Project Settings.

#include "RyuBaseCharacter.h"
#include "Components/RyuClimbingComponent.h"
#include "Components/RyuDebugComponent.h"
#include "Components/RyuMovementComponent.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "ERyuLookDirection.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterState.h"
#include <Camera/CameraComponent.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Math/Vector.h>
#include <PaperZD/Public/AnimSequences/Players/PaperZDAnimPlayer.h>
#include <PaperZD/Public/PaperZDAnimBP.h>
#include <PaperZD/Public/PaperZDAnimInstance.h>

ARyuBaseCharacter::ARyuBaseCharacter()
{
}

ARyuBaseCharacter::ARyuBaseCharacter(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<URyuMovementComponent>(
        ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

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

    bSphereTracerOverlap = false;

    bJumpJustStarted = false;

    TreshholdYWalkRun = 220.0f;

    // Create CharacterState
    // PlayerMovement = EPlayerMovement::STAND;
    CharacterState = NewObject<URyuCharacterIdleState>();
    EquipmentState = NewObject<URyuCharacterIdleState>();
}

void ARyuBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARyuBaseCharacter::AnimationSequenceEnded(const UPaperZDAnimSequence* InAnimSequence)
{
    // TODO CSM Ended stuff link here !
}

void ARyuBaseCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RyuMovementComponent = Cast<URyuMovementComponent>(GetCharacterMovement());
}

void ARyuBaseCharacter::OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                         AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp,
                                                         int32 OtherBodyIndex, bool bFromSweep,
                                                         const FHitResult& SweepResult)
{
    //OnSphereTracerCheckOverlap(OtherActor, OtherComp);
}

void ARyuBaseCharacter::OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
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

void ARyuBaseCharacter::OnSphereTracerCheckOverlap(AActor* OtherActor,
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

        /*

        // TODO to ClimbingStateMachine
		ERYUClimbingMode ClimbState = RyuClimbingComponent->GetClimbingState();

        if ((ClimbState != ERYUClimbingMode::CANCLIMBDOWNLEDGE)
            && (ClimbState != ERYUClimbingMode::CANCLIMBUPANDDOWN)
            && (ClimbState != ERYUClimbingMode::CANCLIMBUPLEDGE)
            && (ClimbState != ERYUClimbingMode::CLIMBDOWNLEDGE)
            && (ClimbState != ERYUClimbingMode::CLIMBUPLEDGE)
            //&& (PlayerMovement != EPlayerMovement::CLIMBING))

        {
            UE_LOG(LogTemp, Log,
                   TEXT("OnSphereTracerCheckOverlap(): SphereTracer->Mode->CanTraceActivated"));
            //PlayerMovement = EPlayerMovementMode::CANTRACELEDGE;
            //PlayerMovement = EPlayerMovement::CANGRABLEDGE;
        }

		*/
    }
}

void ARyuBaseCharacter::SetCharacterMovementState(ERyuMovementState MovementState)
{
    CharacterMovementState = MovementState;
}

void ARyuBaseCharacter::OnHandleCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                    AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp,
                                                    int32 OtherBodyIndex, bool bFromSweep,
                                                    const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleBeginOverlap(): CapslOvlp in from %s"),
           *OtherComp->GetName());
}

void ARyuBaseCharacter::OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                  AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleEndOverlap(): CapslOvlp out from %s"),
           *OtherComp->GetName());
}

bool ARyuBaseCharacter::CheckOverlapClimbableActors()
{
    // TODO ClimbStateMach genauer abchecken nach was geprueft wird: a) climbingActors b) pushactors c) blabla ...

    UE_LOG(LogTemp, Log, TEXT("CheckOverlappingActors(): "));

    auto* ClimbingComp = FindComponentByClass<URyuClimbingComponent>();

    if (!ClimbingComp)
    {
        return false;
    }

    GetOverlappingActors(CapsuleOverlappedActors, ClimbingComp->ClimbableActorClass);

    if (auto* DebugComp = FindComponentByClass<URyuDebugComponent>())
    {
        DebugComp->OutputCapsuleOverlappedComponents(CapsuleOverlappedComponents);
    }

    if (CapsuleOverlappedActors.Num() > 0)
    {
        return CheckOverlappingComponents();
    }
    else
    {
        ClimbingComp->ResetClimbingState();
        return false;
    }
}
/*
Atm this only function when max. 2 climbing trigger are in place. because we automatically want to choose the correct one (up/down, left/right)
*/
// check if an overlPPING actor is there or not and in InputMethod set the state
bool ARyuBaseCharacter::CheckOverlappingComponents()
{
    //BY DESIGN: max. overlapped climbing Triggers == 2
    //check if overlapped comp right or left
    int CountClimbTrigger = CapsuleOverlappedActors.Num();

    if (CountClimbTrigger == 0)
    {
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): %s"),
           *FString::FromInt(CountClimbTrigger));

    if (CountClimbTrigger > 2)
    {
        UE_LOG(LogTemp, Warning,
               TEXT("CheckOverlappingComponents(): To many overlapping ClimbingTrigger (%s). "
                    "Please realign ClimbingAssets!"),
               *FString::FromInt(CountClimbTrigger));
        UE_LOG(LogTemp, Warning, TEXT("CheckOverlappingComponents(): Trigger are:"));
        for (int i = 0; i < CountClimbTrigger; i++)
        {
            UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): %s"),
                   *CapsuleOverlappedActors[i]->GetName());
        }
        return false;
    }
    else
    {
        auto* ClimbingComp = FindComponentByClass<URyuClimbingComponent>();

        if (!ClimbingComp)
        {
            return false;
        }

        ERYULedgeSideEntered LedgeSide = ClimbingComp->GetLedgeSide(0);

        if (CountClimbTrigger == 1)
        {
            //save all Overlapping Components in a array, before that we make sure that the array is empty
            //we know that the TriggerComponents we search for, are UBoxComponents and we do not need any other
            ClimbingComp->GetOverlappingBoxComponents();

            UE_LOG(LogTemp, Log,
                   TEXT("CheckOverlappingComponents(): Yes it´s %s with the Trigger at ... "),
                   *CapsuleOverlappedActors[0]->GetName());
            /* fast check what kind of trigger*/

            if (auto* DebugComp = FindComponentByClass<URyuDebugComponent>())
            {
                DebugComp->OutputCapsuleOverlappedComponents(CapsuleOverlappedComponents);
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

        if (CountClimbTrigger == 2)
        {
            //Check at Input: we know we can Move Up OR Down
            //check if overlapped comp up or/and down
            //check if overlapped comp right or left

            // TODO later call to Character StateMachine !
            ClimbingComp->SetClimbingState(ERYUClimbingMode::CANCLIMBUPANDDOWN);
        }
        return true;
    }
}

void ARyuBaseCharacter::SetClimbingMode(ERYUClimbingMode ClimbingModeToSet)
{
    if (auto* ClimbComp = FindComponentByClass<URyuClimbingComponent>())
    {
        ClimbComp->SetClimbingState(ClimbingModeToSet);
    }
}

void ARyuBaseCharacter::SetLookRight()
{
    FVector currV = GetVelocity();

    if (currV.X > 0)
    {
        LookDirection = ERyuLookDirection::Right;
        //bLookRight = true;
    }
    else if (currV.X < 0)
    {
        LookDirection = ERyuLookDirection::Left;
        //bLookRight = false;
    }
}

ERYUClimbingMode ARyuBaseCharacter::GetClimbingMode()
{
    if (auto* ClimbComp = FindComponentByClass<URyuClimbingComponent>())
    {
        return ClimbComp->GetClimbingState();
    }

    return ERYUClimbingMode::NONE;
}

float ARyuBaseCharacter::GetCharacterStatus(ERyuCharacterStatus Status)
{
    switch (Status)
    {
        case ERyuCharacterStatus::Stamina:
        {
            // TODO Placeholder -> later ask another StatusComponent of CharacterStatus...
            return 100.0f;
            break;
        }
        default:
            return 0.0f;
            break;
    }
}

ERyuInteractionStatus ARyuBaseCharacter::GetInteractionStatus()
{
    // TODO GetInteractionStatus resp. which Objects/Person is colliding; else return None
    // if Movable Object: InteractionStatus = StartMoveObjects
    return InteractionStatus;
}

ERyuMovementState ARyuBaseCharacter::GetCharacterMovementState()
{
    return CharacterMovementState;
}

ERyuLookDirection ARyuBaseCharacter::GetLookDirection()
{
    //return bLookRight;
    return LookDirection;
}

bool ARyuBaseCharacter::IsInCombat()
{
    //TODO check if Character is in a Combat-Situation
    return false;
}

bool ARyuBaseCharacter::EnemyInSight()
{
    return false;
}

void ARyuBaseCharacter::FlipCharacter()
{
    if (Controller != nullptr)
    {
        //if (TravelDirection < 0.0f)
        if (LookDirection == ERyuLookDirection::Right)
        {
            Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
            //CameraBoom->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
            LookDirection = ERyuLookDirection::Left;
        }
        //else if (TravelDirection > 0.0f)
        else
        {
            Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
            //CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
            LookDirection = ERyuLookDirection::Right;
        }
    }

    // LookDirection = !LookDirection;
    //UE_LOG(LogTemp, Log, TEXT("FlipCharacter(): lookRight = %s"),
    //       bLookRight ? TEXT("true") : TEXT("false"));
    //coa vs reset ?
    CheckOverlapClimbableActors();
}

ERyuCharacterState ARyuBaseCharacter::GetCharacterState()
{
    return CharacterState->GetState();
}

void ARyuBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(
        this, &ARyuBaseCharacter::OnHandleCapsuleBeginOverlap);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(
        this, &ARyuBaseCharacter::OnHandleCapsuleEndOverlap);
}

void ARyuBaseCharacter::ConfigurePlayer_Implementation(UPaperZDAnimPlayer* Player)
{
    Super::ConfigurePlayer_Implementation(Player);

    UPaperZDAnimInstance* AnimInstance2D = GetOrCreateAnimInstance();

    Player->OnPlaybackSequenceComplete.AddDynamic(this, &ARyuBaseCharacter::AnimationSequenceEnded);
}

void ARyuBaseCharacter::Jump()
{
}

void ARyuBaseCharacter::StopJumping()
{
}

void ARyuBaseCharacter::SetLookDirection(ERyuLookDirection Direction)
{
    LookDirection = LookDirection;
}
