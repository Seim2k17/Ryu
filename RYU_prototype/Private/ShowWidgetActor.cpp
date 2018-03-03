// Fill out your copyright notice in the Description page of Project Settings.

#include "ShowWidgetActor.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"



// Sets default values
AShowWidgetActor::AShowWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	RootComponent = TriggerBox;

	TriggerBox->SetBoxExtent(FVector(150, 300, 50));

}

void AShowWidgetActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&AShowWidgetActor::HandleBeginOverlap);

	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AShowWidgetActor::HandleEndOverlap);

	

}

void AShowWidgetActor::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Char jumped off"));
	ShowUserWidget();
}

void AShowWidgetActor::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("And dies"));

}

void AShowWidgetActor::ShowUserWidget()
{
	if (WidgetToShow)
	{
		MissionWidgetText = CreateWidget<UUserWidget>(GetWorld(), WidgetToShow);

		if (MissionWidgetText)
		{
			MissionWidgetText->AddToViewport();
			MissionCompleted();
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("No User Widget-Reference linked."));

}

void AShowWidgetActor::HideUserWidget()
{
	MissionWidgetText->RemoveFromViewport();

}
