// Fill out your copyright notice in the Description page of Project Settings.

#include "RyuBaseCharacter.h"
#include "Components/RyuMovementComponent.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"
#include <Camera/CameraComponent.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Math/Vector.h>

ARyuBaseCharacter::ARyuBaseCharacter()
{
}

ARyuBaseCharacter::ARyuBaseCharacter(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<URyuMovementComponent>(
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

    

    bSphereTracerOverlap = false;

    bJumpJustStarted = false;

    TreshholdYWalkRun = 220.0f;
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

void ARyuBaseCharacter::CheckOverlappingActors()
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

void ARyuBaseCharacter::CheckOverlappingComponents()
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
                   TEXT("CheckOverlappingComponents(): Yes it�s %s with the Trigger at ... "),
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



void ARyuBaseCharacter::OutputCapsuleOverlappedComponents()
{
    for (int i = 0; i < CapsuleOverlappedComponents.Num(); i++)
    {
        UPrimitiveComponent* el = CapsuleOverlappedComponents[i];
        UE_LOG(LogTemp, Log, TEXT("OutputCapsuleOverlapArray[%s]: %s Posi: %s Owner: %s"),
               *FString::FromInt(i), *el->GetName(), *el->GetOwner()->GetActorLocation().ToString(),
               *el->GetOwner()->GetName());
    }
}

void ARyuBaseCharacter::SetPlayerMovement(EPlayerMovement PlayerStateToSet)
{
    PlayerMovement = PlayerStateToSet;
}

EPlayerMovement ARyuBaseCharacter::GetPlayerMovement()
{
    return PlayerMovement;
}

void ARyuBaseCharacter::SetClimbingMode(ERYUClimbingMode ClimbingModeToSet)
{
    RYUClimbingMode = ClimbingModeToSet;
}

ERYUClimbingMode ARyuBaseCharacter::GetClimbingMode()
{
    return RYUClimbingMode;
}

void ARyuBaseCharacter::FlipCharacter()
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





void ARyuBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(
        this, &ARyuBaseCharacter::OnHandleCapsuleBeginOverlap);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(
        this, &ARyuBaseCharacter::OnHandleCapsuleEndOverlap);
}

/** General Movement Stuff*/

void ARyuBaseCharacter::MoveRight(float Val)
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

void ARyuBaseCharacter::Jump()
{
}

void ARyuBaseCharacter::StopJumping()
{
}
