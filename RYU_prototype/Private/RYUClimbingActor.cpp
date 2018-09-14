// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUClimbingActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "RYUCharacterIchi.h"
#include "Engine/TargetPoint.h"
#include "Components/ArrowComponent.h"


// Sets default values
ARYUClimbingActor::ARYUClimbingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;

	ClimbMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClimbableMesh"));
	ClimbMeshComponent->SetupAttachment(Root);

	IsRightUpTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("IsRightUpTrigger"));
	IsRightUpTrigger->SetupAttachment(Root);
	
	IsRightDownTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("IsRightDownTrigger"));
	IsRightDownTrigger->SetupAttachment(Root);

	IsLeftUpTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("IsLeftUpTrigger"));
	IsLeftUpTrigger->SetupAttachment(Root);

	IsLeftDownTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("IsLeftDownTrigger"));
	IsLeftDownTrigger->SetupAttachment(Root);

	LeftHangPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftHangPosition"));
	LeftHangPosition->SetupAttachment(Root);

	RightHangPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightHangPosition"));
	RightHangPosition->SetupAttachment(Root);

	DownLeftStandPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("DownLeftStandPosition"));
	DownLeftStandPosition->SetupAttachment(Root);

	DownRightStandPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("DownRightStandPosition"));
	DownRightStandPosition->SetupAttachment(Root);

	UpRightStandPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("UpRightStandPosition"));
	UpRightStandPosition->SetupAttachment(Root);

	UpLeftStandPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("UpLeftStandPosition"));
	UpLeftStandPosition->SetupAttachment(Root);

	ClimbingTriggerDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("ClimbingTriggerDirectionGap"));
	ClimbingTriggerDirection->SetupAttachment(Root);
	
	InitializeValues();

}

void ARYUClimbingActor::InitializeValues()
{
	CanClimbUpTag = "CanClimbUp";
	CanClimbDownTag = "CanClimbDown";

	LeftPositionTag = "Left";
	RightPositionTag = "Right";

	bLeftDownTriggerIsActive = true;
	bLeftUpTriggerIsActive = true;
	bRightUpTriggerIsActive = true;
	bRightDownTriggerIsActive = true;

	FVector BoxStartExtend = FVector(20.0f, 125.0f, 20.0f);
	FVector StartLeftUp = FVector(-10.0f, 0.0f, 10.0f);
	FVector StartLeftDown = FVector(-10.0f, 0.0f, -35.0f);
	FVector StartRightUp = FVector(35.0f, 0.0f, 10.0f);
	FVector StartRightDown = FVector(35.0f, 0.0f, -35.0f);

	IsLeftDownTrigger->SetBoxExtent(BoxStartExtend);
	IsRightDownTrigger->SetBoxExtent(BoxStartExtend);
	IsRightUpTrigger->SetBoxExtent(BoxStartExtend);
	IsLeftUpTrigger->SetBoxExtent(BoxStartExtend);

	IsLeftDownTrigger->SetRelativeLocation(StartLeftDown);
	IsRightUpTrigger->SetRelativeLocation(StartRightUp);
	IsRightDownTrigger->SetRelativeLocation(StartRightDown);
	IsLeftUpTrigger->SetRelativeLocation(StartLeftUp);

	IsLeftDownTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleBeginOverlap);
	IsRightUpTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleBeginOverlap);
	IsLeftDownTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleEndOverlap);
	IsRightUpTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleEndOverlap);

	IsRightDownTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleBeginOverlap);
	IsLeftUpTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleBeginOverlap);
	IsRightDownTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleEndOverlap);
	IsLeftUpTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleEndOverlap);

	LeftHangPosition->SetRelativeLocation(FVector(0, 90.0f, 0));
	RightHangPosition->SetRelativeLocation(FVector(0, -210.0f, 0));

	IsLeftDownTrigger->ComponentTags.Add(CanClimbUpTag);
	IsRightDownTrigger->ComponentTags.Add(CanClimbUpTag);
	IsRightUpTrigger->ComponentTags.Add(CanClimbDownTag);
	IsLeftUpTrigger->ComponentTags.Add(CanClimbDownTag);

	IsLeftDownTrigger->ComponentTags.Add(LeftPositionTag);
	IsRightUpTrigger->ComponentTags.Add(RightPositionTag);
	
	IsRightDownTrigger->ComponentTags.Add(RightPositionTag);
	IsLeftUpTrigger->ComponentTags.Add(LeftPositionTag);

	ClimbingTriggerDirection->SetArrowColor(FLinearColor::Yellow);
	//@ToDo: later show with GameDebugVariable 
	ClimbingTriggerDirection->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void ARYUClimbingActor::BeginPlay()
{
	Super::BeginPlay();

	SetVisibilityTrigger();
	
}

void ARYUClimbingActor::OnTriggerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//move it to the player because mostly the player enters the trigger not the platform enters the player except it´s moving but not for now
// 	ARYUCharacterIchi* MyChar = Cast<ARYUCharacterIchi>(OtherActor);
// 	if (MyChar)
// 	{
// 		//MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;
// 		
// 		UCapsuleComponent* MyOvComp = Cast<UCapsuleComponent>(OtherComp);
// 
// 		TArray<AActor*> OverlappedActors;
// 		MyChar->GetOverlappingActors(OverlappedActors, TSubclassOf<ARYUClimbingActor>());
// 		bool CheckUpDownActor = false;
// 
// 		if(MyOvComp == MyChar->GetCapsuleComponent()) 
// 		{
// 			if (OverlappedComponent->ComponentTags[0] == CanClimbDownTag)
// 			{
// 				UE_LOG(LogTemp, Log, TEXT("CanClimbDown TAG: Overlap In"));
// 				MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBDOWNLEDGE;
// 			}
// 
// 			if (OverlappedComponent->ComponentTags[0] == CanClimbUpTag)
// 			{
// 				UE_LOG(LogTemp, Log, TEXT("CanClimbUp TAG: Overlap In"));
// 				MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPLEDGE;
// 			}
// 
// 			//easy check but u need to bee careful !!!
// 			if (OverlappedActors.Num() > 1)
// 			{
// 				MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;
// 			}
// 
// 			if (OverlappedComponent->ComponentTags[1] == LeftPositionTag)
// 			{
// 				MyChar->SetLedgeHangPosition(LeftHangPosition->GetComponentLocation(), LeftPositionTag);
// 				UE_LOG(LogTemp, Log, TEXT("TAG: %s"),*LeftPositionTag.ToString());
// 			}
// 
// 			if (OverlappedComponent->ComponentTags[1] == RightPositionTag)
// 			{
// 				MyChar->SetLedgeHangPosition(RightHangPosition->GetComponentLocation(), RightPositionTag);
// 				UE_LOG(LogTemp, Log, TEXT("TAG: %s"), *RightPositionTag.ToString());
// 			}
// 		}
// 		
// 	}
}

void ARYUClimbingActor::OnTriggerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	/************************************************************************/
	/* Hmmm move it to the player                                                                      */
	/************************************************************************/
// 	ARYUCharacterIchi* MyChar = Cast<ARYUCharacterIchi>(OtherActor);
// 	if (MyChar)
// 	{
// 
// 		//MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;
// 
// 		USphereComponent* MyOvComp = Cast<USphereComponent>(OtherComp);
// 
// 		if (MyOvComp == MyChar->SphereTracer)
// 		{
// 			UE_LOG(LogTemp, Log, TEXT("CanClimbDown TAG: Overlap Out"));
// 			MyChar->RYUClimbingMode = ERYUClimbingMode::NONE;
// 			MyChar->RYUMovement = ERYUMovementMode::STAND;
// 
// 			MyChar->SetLedgeHangPosition(FVector::ZeroVector, "none");
// 		}
// 
// 		
// 	}
		
}

#if WITH_EDITOR
void ARYUClimbingActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetVisibilityTrigger();
}

#endif


void ARYUClimbingActor::SetVisibilityTrigger()
{
	IsLeftDownTrigger->SetVisibility(bLeftDownTriggerIsActive);
	IsLeftDownTrigger->bGenerateOverlapEvents = bLeftDownTriggerIsActive;

	IsLeftUpTrigger->SetVisibility(bLeftUpTriggerIsActive);
	IsLeftUpTrigger->bGenerateOverlapEvents = bLeftUpTriggerIsActive;

	IsRightDownTrigger->SetVisibility(bRightDownTriggerIsActive);
	IsRightDownTrigger->bGenerateOverlapEvents = bRightDownTriggerIsActive;

	IsRightUpTrigger->SetVisibility(bRightUpTriggerIsActive);
	IsRightUpTrigger->bGenerateOverlapEvents = bRightUpTriggerIsActive;
}

// Called every frame
void ARYUClimbingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

