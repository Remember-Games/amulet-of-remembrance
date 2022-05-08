// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "AORBaseCharacter.h"
#include "AORCharacterMemory.h"
#include "AORShadowController.generated.h"

/**
 * 
 */
UCLASS()
class AMULETOFREMEMBRANCE_API AAORShadowController : public AAIController
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = FollowForce)
	double Spring;
	
	UPROPERTY(EditDefaultsOnly, Category = FollowForce)
	double Damper;
	
	UPROPERTY(EditDefaultsOnly, Category = FollowForce)
	double MaxDistance;
	
	AAORShadowController();

public:
	virtual void OnPossess(APawn* inPawn) override;
	virtual void Tick(float deltaTime) override;
	void SetNewMemory(AORCharacterMemory& memory);

private:
	void DoMovements(FTimespan ts, float deltaTime);
	void ApplyCorrectionForce(FVector targetPos, FVector targetVel, float deltaTime);
	void DoActions(FTimespan ts);

private:
	AAORBaseCharacter* character;
	AORCharacterMemory memory;
	int mInd;
	int aInd;
	FDateTime beginMemTime;
	
	bool isCorrectionForce;
	FVector lastTargetPos;
	UCharacterMovementComponent* characterMovement;
};
