// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RYUGameMode.generated.h"


DECLARE_DELEGATE(FStandardDelegateSignature)
/**
 * 
 */
UCLASS()
class RYU_PROTOTYPE_API ARYUGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	FStandardDelegateSignature RYUStandardSignature;
	
	
};
