// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RyuBaseCommand.generated.h"

/**
 * This is the Base Class of the Command-Pattern (http://gameprogrammingpatterns.com/command.html)
 */
UCLASS()
class RYU_PROTOTYPE_API URyuBaseCommand : public UObject
{
	GENERATED_BODY()
	
public:
	virtual ~URyuBaseCommand() {};
	virtual void Execute() {};
};
