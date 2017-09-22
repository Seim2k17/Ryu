// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTestActor.h"


// Sets default values
AMyTestActor::AMyTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTestActor::BeginPlay()
{
	Super::BeginPlay();

	//ways to make Output Log in UE4C++

	UE_LOG(LogTemp, Warning, TEXT("WARNING! u enter c++ zone"));
	int intVar = 5;
	float floatVar = 0.5f;
	FString fstringVar = "an Fstring Variable";
	UE_LOG(LogTemp, Warning, TEXT("BlaBla, %d %f %s"), intVar, floatVar, *fstringVar);

	FString name = "Ragnar";
	int32 mana = 450;
	TArray <FStringFormatArg> args;
	
	args.Add(FStringFormatArg(name));
	args.Add(FStringFormatArg(mana));
	FString string = FString::Printf(TEXT("Name = %s Mana = %d"), *name, mana);
	UE_LOG(LogTemp, Warning, TEXT("Bla: %s"),*string);
	FString string2 = FString::Format(TEXT("Name = {0} Mana = {1}"), args);
	UE_LOG(LogTemp, Warning, TEXT("So also siehts hier aus: %s"), *string2);
	
}

// Called every frame
void AMyTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

