// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUPressurePlate.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "RYUGameMode.h"
#include "PushableBoxDelegatSymbol.h"
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SingleDoorActor.h"


// Sets default values
ARYUPressurePlate::ARYUPressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");

	RootComponent = SceneComp;
	
	TriggerZone = CreateDefaultSubobject<UBoxComponent>("Triggerzone");
	TriggerZone->SetupAttachment(SceneComp);
	
	BorderMesh = CreateDefaultSubobject<UStaticMeshComponent>("BorderMesh");
	BorderMesh->SetupAttachment(SceneComp);
	PressurePlateMesh = CreateDefaultSubobject<UStaticMeshComponent>("PressurePlateMesh");
	PressurePlateMesh->SetupAttachment(SceneComp);
	//
	
	//TestBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TESTBOX"));
	//TestBox->SetupAttachment(TriggerZone);

	MovingPlateComp = CreateDefaultSubobject<UPlateMovingComponent>("MovingComponent");

	auto PressurePlateMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/3d_model/blockOut/blockOut_Pressure_plate_normal.blockOut_Pressure_plate_normal'"));
	auto BorderMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/3d_model/blockOut/blockOut_CutOutPlate.blockOut_CutOutPlate'"));
	

	if (PressurePlateMeshAsset.Object != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PRESSUREPLATEMESHASSET SET."))
		PressurePlateMesh->SetStaticMesh(PressurePlateMeshAsset.Object);
				
	}

	if (BorderMeshAsset.Object != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BORDERMESHASSET SET."));
		BorderMesh->SetStaticMesh(BorderMeshAsset.Object);
		
	}
	
}

// Called when the game starts or when spawned
void ARYUPressurePlate::BeginPlay()
{
	Super::BeginPlay();
	BorderMesh->SetRelativeLocation(FVector(0, 0, 0));
	PressurePlateMesh->SetRelativeLocation(MovingPlateComp->PositionPlateOffset);
	
	if (TriggerZone != nullptr)
	{
		TriggerZone->SetRelativeLocation(FVector(0, 0, 54));
		TriggerZone->SetBoxExtent(FVector(105, 105, 32));
	}
	

	auto TheWorld = (UObject*)GetWorld();
	
	if (TheWorld != nullptr)
	{
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ARYUGameMode* RYUGameMode = Cast<ARYUGameMode>(GameMode);
		if (RYUGameMode)
		{
			//RYUGameMode->RYUBlockTypeDelegate.BindUObject(this, &ARYUPressurePlate::SetBoxTypeOnPlate);
		}
	}
			
}

// Called every frame
void ARYUPressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARYUPressurePlate::NotifyActorBeginOverlap(AActor* otherActor)
{
	++MovingPlateComp->ActorsOnPlate;
	UE_LOG(LogTemp, Display, TEXT("Actor entered Triggerzone: %s"), *otherActor->GetName());
	auto* comp = otherActor->FindComponentByClass<UPushableBoxDelegatSymbol>();
	if (comp != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerdActor is: %s "), *otherActor->GetName());
		MovingPlateComp->SetTriggeredActor(otherActor);
	}
	if (MovingPlateComp->ActorsOnPlate == 1)
	{
		MovingPlateComp->MoveDown = true;
		MovingPlateComp->MoveUp = false;
	}

	auto TheWorld = (UObject*)GetWorld();

	if (TheWorld != nullptr)
	{
		AGameModeBase *GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ARYUGameMode * RYUGameMode = Cast<ARYUGameMode>(GameMode);
		if (RYUGameMode)
		{
			RYUGameMode->RYUStandardDelegate.ExecuteIfBound();
		}
		
	}
	
}

/*
FText ARYUPressurePlate::GetSymbolEnumAsString(ESymbolNames EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EVictoryEnum"), true);
	if (!EnumPtr) return NSLOCTEXT("Invalid", "Invalid", "Invalid");

	return EnumPtr->GetDisplayNameText(EnumValue);
}
*/

void ARYUPressurePlate::NotifyActorEndOverlap(AActor* otherActor)
{
	--MovingPlateComp->ActorsOnPlate;
	UE_LOG(LogTemp, Display, TEXT("Actor left Triggerzone: %s"), *otherActor->GetName());
	auto* comp = otherActor->FindComponentByClass<UPushableBoxDelegatSymbol>();
	if (comp != nullptr)
	{
		if (MovingPlateComp->ActivateSomething)
			{
				MovingPlateComp->SetTriggeredActor(nullptr);
				MovingPlateComp->ActivateSomething = false;

				auto TheWorld = (UObject*)GetWorld();
				AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
				ARYUGameMode* RYUGameMode = (ARYUGameMode*)(GameMode);
				if (RYUGameMode != nullptr)
				{
					RYUGameMode->RYUTriggerSomething.Broadcast();
				}
				UE_LOG(LogTemp, Warning, TEXT("You hear a RE-click."));
				//ClientMessage("CLICK-CLICK");
			}
		
	}
	

	if (MovingPlateComp->ActorsOnPlate == 0)
	{
		MovingPlateComp->MoveDown = false;
		MovingPlateComp->MoveUp = true;
		MovingPlateComp->SetTriggeredActor(nullptr);
	}
}

void ARYUPressurePlate::SetBoxTypeOnPlate(FString symbolOnBox)
{
	UE_LOG(LogTemp, Display, TEXT("Symbol on Box : %s"),*symbolOnBox);
}



