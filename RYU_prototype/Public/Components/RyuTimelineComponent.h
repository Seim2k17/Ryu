// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include <Components/TimelineComponent.h>
#include "RyuTimelineComponent.generated.h"

class UCurveFloat;
class UCurveVector;
class ARyuMainCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RYU_PROTOTYPE_API URyuTimelineComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URyuTimelineComponent();

    void PlayTimeline();

    void SetCurrentTimelineParamsFloat(UCurveFloat* FloatCurveX, UCurveFloat* FloatCurveZ,
                                       bool TimelineIsLooping, bool IgnoreTimeDilation);

    void SetCurrentTimelineParamsVector(UCurveVector* VectorCurve, bool TimelineIsLooping,
                                        bool IgnoreTimeDilation);

protected:
    void BeginPlay() override;

    //Updatefunction for the Timeline
    UFUNCTION()
    void TimelineCallbackFloat(float val);

    UFUNCTION()
    void TimelineCallbackVector(FVector Vec);

    //When Finished the Timeline
    UFUNCTION()
    void TimelineFinishedCallback();

    /** onTimelineCallback contains the signature of the function that is going to execute every time we tick our timeline.
	 *	Think of onTimelineCallback as a delegate!
     * Declare our delegate function to be binded with TimelineCallback 
	 */
    FOnTimelineFloat onTimelineCallbackFloat{};

    FOnTimelineVector onTimelineCallbackVector{};

    //FOnTimelineEventStatic onTimelineFinishedCallback;

    /** Declare our delegateFunction to be binded to TimelineFinishedCallback()*/
    FOnTimelineEvent onTimelineFinishedCallback{};

    //** TimeLine Stuff */
    UPROPERTY()
    TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

    UPROPERTY()
    UTimelineComponent* CurrentTimeline;

public:
    void TickComponent(float DeltaTime, ELevelTick TickType,
                       FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, Category = "TimelineData")
    UCurveFloat* JumpUpFloatCurve;

    UPROPERTY(EditAnywhere, Category = "TimelineData")
    UCurveVector* JumpForwardVectorCurve;

    UPROPERTY(EditAnywhere, Category = "TimelineData")
    UCurveVector* ClimbUpVectorCurve;

    UPROPERTY(VisibleAnywhere, Category = "TimelineData")
    FVector StartTimelineLocation;

    UPROPERTY(VisibleAnywhere, Category = "TimelineData")
    FVector EndTimelineLocation;

    UPROPERTY(EditAnywhere, Category = "TimelineData")
    float ClimbUpOffsetX;

    UPROPERTY(EditAnywhere, Category = "TimelineData")
    float ClimbUpOffsetZ;

private:
    ARyuMainCharacter* GetMainCharOwner();
};
