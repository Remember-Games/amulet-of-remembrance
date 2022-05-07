// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	AAORShadowController();

public:
	virtual void OnPossess(APawn* inPawn) override;
	virtual void Tick(float deltaTime) override;
	void SetNewMemory(AORCharacterMemory& memory);

private:
	void DoMovements(FTimespan ts);
	void DoActions(FTimespan ts);

private:
	AAORBaseCharacter* character;
	AORCharacterMemory memory;
	int mInd;
	int aInd;
	FDateTime beginMemTime;
	
	const double MAX_CORRECTION_POS_MAG = 100;
	bool isPositionMatched;


};
