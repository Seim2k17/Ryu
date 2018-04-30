// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUHealthComponent.h"


// Sets default values for this component's properties
URYUHealthComponent::URYUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//LOOK:
	//only forDebugOutput remove later when HUD is final
	PrimaryComponentTick.bCanEverTick = true;

	bIsDead = false;
	DefaultHealth = 100.f;


	// ...
}


// Called when the game starts
void URYUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		//Handles all the Damage/Bind to Member
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &URYUHealthComponent::HandleTakeAnyDamage);
	}

	//initialize Health
	Health = DefaultHealth;
	
}


void URYUHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || bIsDead)
	{
		return;
	}

	//Updates Health
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Log, TEXT("Damage: %s, current Health: %s"),*FString::SanitizeFloat(Damage),*FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType,InstigatedBy,DamageCauser);

	if (bIsDead)
	{
		//Do Killingstuff
	}
}

// Called every frame
//LOOK:
//only forDebugOutput remove later when HUD is final
void URYUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

