// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Delegate.h"
#include "CoreMinimal.h"
#include "RYUGameMode.generated.h"


DECLARE_DELEGATE(FStandardDelegateSignature);
DECLARE_DELEGATE_OneParam(FParamDelegateListener, FString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggerSomething);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterCollider);

UCLASS()
class RYU_PROTOTYPE_API ARYUGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ARYUGameMode();

	FStandardDelegateSignature RYUStandardDelegate;

	FParamDelegateListener RYUBlockTypeDelegate;

	//UPROPERTY(BlueprintReadWrite)
	//FMulticastDelegateSignature RYUOpenCloseDoorDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	//FOnOpenCloseDoor RYUOpenCloseDoorDelegate;
	FOnTriggerSomething RYUTriggerSomething;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEnterCollider RYUEnterSplineCollider;
	
	

};
