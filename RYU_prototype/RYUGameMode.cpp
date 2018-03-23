// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUGameMode.h"
#include "ConstructorHelpers.h"
#include "WalkSplineComponent.h"
#include "Runtime/Engine/Classes/Engine/Blueprint.h"

ARYUGameMode::ARYUGameMode()
{
	
	//static ConstructorHelpers::FObjectFinder<UBlueprint> SideScroller_BP(TEXT("Blueprint'/Game/SideScrollerBP/Blueprints/SideScrollerCharacter.SideScrollerCharacter'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> SideScroller_BP(TEXT("Blueprint'/Game/Blueprints/Character/RYU_Character/BP_Character_base.BP_Character_base'"));
	if (SideScroller_BP.Object) {
		DefaultPawnClass = (UClass*)SideScroller_BP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerController_BP(TEXT("Blueprint'/Game/Blueprints/Controller/RYU_PlayerController_BP.RYU_PlayerController_BP'"));
	if (PlayerController_BP.Object) {
		PlayerControllerClass = (UClass*)PlayerController_BP.Object->GeneratedClass;
	}

	// AddDynamic( ... FOneEnterCollider RYUEnterSplineCollider; ) um Funktionalitaet zu binden ....

	/* Standard C++ 
	DefaultPawnClass = ASidescroll::StaticClass();
	PlayerControllerClass = ASomePlayerController::StaticClass();
	*/
}
