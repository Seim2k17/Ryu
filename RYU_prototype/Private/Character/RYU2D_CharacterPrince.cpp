// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_CharacterPrince.h"




ARYU2D_CharacterPrince::ARYU2D_CharacterPrince(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URYU2D_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	CreateDefaultSubobject<UClimbAssetComponent>(TEXT("ClimbAssetComponent"));

	InitializeCharacterValues();
}

void ARYU2D_CharacterPrince::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MovementComp = Cast<URYU2D_MovementComponent>(GetCharacterMovement());
}

void ARYU2D_CharacterPrince::Jump()
{

}

void ARYU2D_CharacterPrince::StopJumping()
{

}

void ARYU2D_CharacterPrince::Tick(float DeltaTime)
{

}

void ARYU2D_CharacterPrince::DrawDebugInfosOnScreen()
{

}

void ARYU2D_CharacterPrince::DebugSomething()
{

}

void ARYU2D_CharacterPrince::Climb(float Val)
{

}

void ARYU2D_CharacterPrince::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

}

void ARYU2D_CharacterPrince::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

}

void ARYU2D_CharacterPrince::MoveRight(float Val)
{

}

void ARYU2D_CharacterPrince::InitializeCharacterValues()
{
	throw std::logic_error("The method or operation is not implemented.");
}
