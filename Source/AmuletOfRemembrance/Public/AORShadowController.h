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
	void DoRotations(FTimespan ts);
	void DoActions(FTimespan ts);
	void DoCorrections(FTimespan ts);

private:
	AAORBaseCharacter* character;
	AORCharacterMemory memory;
	int movementsInd;
	int rotationsInd;
	int actionsInd;
	int correctionsInd;
	FDateTime beginMemTime;
	
	const double MAX_CORRECTION_POS_MAG = 0.1;
	bool isCorrected;

};
