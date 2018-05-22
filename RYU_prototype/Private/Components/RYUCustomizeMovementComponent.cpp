// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUCustomizeMovementComponent.h"


// Sets default values for this component's properties
URYUCustomizeMovementComponent::URYUCustomizeMovementComponent()
{
	
	//mostly just a Data Component
	VelocityAfterJumping.X = 0.0f;
	VelocityAfterJumping.Y = 450.0f;
	VelocityAfterJumping.Z = 0.0f;
	

	AfterJumpTreshold.X = 0.0f;
	AfterJumpTreshold.Y = 100.0f;
	AfterJumpTreshold.Z = -950.0f;

	JumpForce.X = 0.0f;
	JumpForce.Y = 300.0f;
	JumpForce.Z = 0.0f;

	JumpForceRun.X = 0.0f;
	JumpForceRun.Y = 0.0f;
	JumpForceRun.Z = 0.0f;

	AddFallingMultiplierNumber = 0.05f;
	GravityScaleMaximum = 4.0f;
}


// Called when the game starts
void URYUCustomizeMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}



void URYUCustomizeMovementComponent::SetGravityScaleMaximum(float GravScale)
{
	GravityScaleMaximum = GravScale;
}

