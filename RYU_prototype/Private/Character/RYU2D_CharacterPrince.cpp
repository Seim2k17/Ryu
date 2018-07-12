// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_CharacterPrince.h"
#include "PaperFlipbookComponent.h"
#include "RYU2D_MovementComponent.h"
#include "ClimbAssetComponent.h"




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

	//@ToDo: take closer look when CustomMovement will be impl.
	//MovementComp = Cast<URYU2D_MovementComponent>(GetCharacterMovement());
}


void ARYU2D_CharacterPrince::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARYU2D_CharacterPrince::MoveRight);
	PlayerInputComponent->BindAxis("Climb", this, &ARYU2D_CharacterPrince::MoveUp);
}

void ARYU2D_CharacterPrince::MoveRight(float Val)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	if (Val != 0)
	{
		UE_LOG(LogTemp, Log, TEXT("MOVE: %s ; bLookRight: %s ; bPlayTurnAni: %s ; Sprite(looping) %s"), *FString::SanitizeFloat(Val),
			bLookRight ? TEXT("true") : TEXT("false"), bPlayTurnAni ? TEXT("true") : TEXT("false"), GetSprite()->IsLooping() ? TEXT("true") : TEXT("false"));

	}

	switch (PlayerMovement)
	{
		//@ToDo: easeIn
	case EPlayerMovement::BEGINRUN:
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val * 0.5f);
		break;
		//@ToDo: easeOut
	case EPlayerMovement::ENDRUN:
		float MoveValue = bLookRight ? 0.06f : -0.06f;
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveValue);
		break;
	}

	//needs a deeper look
	if ((PlayerMovement != EPlayerMovement::STARTTURN) &&
		(!bStartedNoLoopAnimation))
	{
		if ((bLookRight && Val > 0) || (!bLookRight && Val < 0))
		{
			AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val);

			switch (PlayerMovement)
			{
			case EPlayerMovement::STAND:
				PlayerMovement = EPlayerMovement::BEGINRUN;
				break;
			case EPlayerMovement::JUMPUP:
				//@ToDo
				break;
			case EPlayerMovement::FALLING:
				//@ToDo
				break;
			case EPlayerMovement::HANGINGLEDGE:
				//@ToDo
				break;
			case EPlayerMovement::CLIMBING:
				//@ToDo
				break;
			case EPlayerMovement::SNEAK:
				//@ToDo
				break;
			default:
				break;
			}
		}
		else
		{
			if (Val != 0)
			{
				PlayerMovement = EPlayerMovement::STARTTURN;
			}
			else
			{
				//just released key (needs another deeper look with Controller because COntrollerX = Axis
				if (PlayerMovement == EPlayerMovement::RUN)
				{
					PlayerMovement = EPlayerMovement::ENDRUN;
				}
				else
				{
					if (!bStartedNoLoopAnimation) PlayerMovement = EPlayerMovement::STAND;
				}

			}

		}
	}
}

void ARYU2D_CharacterPrince::MoveUp(float Value)
{

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



void ARYU2D_CharacterPrince::InitializeCharacterValues()
{
	
}
