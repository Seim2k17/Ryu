// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUClimbingActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "RYUCharacterIchi.h"
#include "Engine/TargetPoint.h"


// Sets default values
ARYUClimbingActor::ARYUClimbingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;

	ClimbMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClimbableMesh"));
	ClimbMeshComponent->SetupAttachment(Root);

	LeftDownTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftDownTrigger"));
	LeftDownTrigger->SetupAttachment(Root);

	LeftUpTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftUpTrigger"));
	LeftUpTrigger->SetupAttachment(Root);

	RightDownTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("RightDownTrigger"));
	RightDownTrigger->SetupAttachment(Root);

	RightUpTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("RightUpTrigger"));
	RightUpTrigger->SetupAttachment(Root);

	LeftHangPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftHangPosition"));
	LeftHangPosition->SetupAttachment(Root);

	RightHangPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightHangPosition"));
	RightHangPosition->SetupAttachment(Root);
	
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

	FVector BoxStartExtend = FVector(70.0f, 30.0f, 15.0f);
	FVector StartLeftUp = FVector(0.0f, 80.0f, -35.0f);
	FVector StartLeftDown = FVector(0.0f, 80.0f, 10.0f);
	FVector StartRightUp = FVector(0.0f, -205.0f, -35.0f);
	FVector StartRightDown = FVector(0.0f, -205.0f, 10.0f);

	LeftUpTrigger->SetBoxExtent(BoxStartExtend);
	RightUpTrigger->SetBoxExtent(BoxStartExtend);
	LeftDownTrigger->SetBoxExtent(BoxStartExtend);
	RightDownTrigger->SetBoxExtent(BoxStartExtend);

	LeftUpTrigger->SetRelativeLocation(StartLeftUp);
	LeftDownTrigger->SetRelativeLocation(StartLeftDown);
	RightUpTrigger->SetRelativeLocation(StartRightUp);
	RightDownTrigger->SetRelativeLocation(StartRightDown);

	LeftUpTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleBeginOverlap);
	LeftDownTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleBeginOverlap);
	LeftUpTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleEndOverlap);
	LeftDownTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleEndOverlap);

	RightUpTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleBeginOverlap);
	RightDownTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleBeginOverlap);
	RightUpTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleEndOverlap);
	RightDownTrigger->OnComponentEndOverlap.AddDynamic(this, &ARYUClimbingActor::OnTriggerHandleEndOverlap);

	LeftHangPosition->SetRelativeLocation(FVector(0, 90.0f, 0));
	RightHangPosition->SetRelativeLocation(FVector(0, -210.0f, 0));

	LeftUpTrigger->ComponentTags.Add(CanClimbUpTag);
	RightUpTrigger->ComponentTags.Add(CanClimbUpTag);
	LeftDownTrigger->ComponentTags.Add(CanClimbDownTag);
	RightDownTrigger->ComponentTags.Add(CanClimbDownTag);

	LeftUpTrigger->ComponentTags.Add(LeftPositionTag);
	LeftDownTrigger->ComponentTags.Add(LeftPositionTag);
	
	RightUpTrigger->ComponentTags.Add(RightPositionTag);
	RightDownTrigger->ComponentTags.Add(RightPositionTag);
}

// Called when the game starts or when spawned
void ARYUClimbingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARYUClimbingActor::OnTriggerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ARYUCharacterIchi* MyChar = Cast<ARYUCharacterIchi>(OtherActor);
	if (MyChar)
	{
		//MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;
		
		UCapsuleComponent* MyOvComp = Cast<UCapsuleComponent>(OtherComp);

		
		if(MyOvComp == MyChar->GetCapsuleComponent()) 
		{
			if (OverlappedComponent->ComponentTags[0] == CanClimbDownTag)
			{
				UE_LOG(LogTemp, Log, TEXT("CanClimbDown TAG: Overlap In"));
				MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBDOWNLEDGE;
			}

			if (OverlappedComponent->ComponentTags[0] == CanClimbUpTag)
			{
				UE_LOG(LogTemp, Log, TEXT("CanClimbUp TAG: Overlap In"));
				MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPLEDGE;
			}

			if (OverlappedComponent->ComponentTags[1] == LeftPositionTag)
			{
				MyChar->SetLedgeHangPosition(LeftHangPosition->GetComponentLocation(), LeftPositionTag);
				UE_LOG(LogTemp, Log, TEXT("TAG: %s"),*LeftPositionTag.ToString());
			}

			if (OverlappedComponent->ComponentTags[1] == RightPositionTag)
			{
				MyChar->SetLedgeHangPosition(RightHangPosition->GetComponentLocation(), RightPositionTag);
				UE_LOG(LogTemp, Log, TEXT("TAG: %s"), *RightPositionTag.ToString());
			}
		}
		
	}
}

void ARYUClimbingActor::OnTriggerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARYUCharacterIchi* MyChar = Cast<ARYUCharacterIchi>(OtherActor);
	if (MyChar)
	{

		//MyChar->RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPANDDOWN;

		USphereComponent* MyOvComp = Cast<USphereComponent>(OtherComp);

		if (MyOvComp == MyChar->SphereTracer)
		{
			UE_LOG(LogTemp, Log, TEXT("CanClimbDown TAG: Overlap Out"));
			MyChar->RYUClimbingMode = ERYUClimbingMode::NONE;

			MyChar->SetLedgeHangPosition(FVector::ZeroVector, "none");
		}

		
	}
		
}

// Called every frame
void ARYUClimbingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

