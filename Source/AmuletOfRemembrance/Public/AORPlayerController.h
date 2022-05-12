// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AORBaseCharacter.h"
#include "AORCharacterMemory.h"
#include "AORPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AMULETOFREMEMBRANCE_API AAORPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = ShadowRecording)
	TSubclassOf<AAORBaseCharacter> ShadowCharacter;
	
	UPROPERTY(EditAnywhere, Category = ShadowRecording)
	float MaxRecordingDuration;
	
	UPROPERTY(EditAnywhere, Category = ShadowRecording)
	int MaxActiveShadows;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	float TurnRateGamepad;
	
	AAORPlayerController();
		
	/*METHODS*/
public:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* inPawn) override;
	virtual void Tick(float deltaTime) override;

private:
	void BeginShadowRecord();
	void StopShadowRecord();

	void ForwardAxis(float value);
	void SidewaysAxis(float value);
	void PitchAxisDelta(float value);
	void YawAxisDelta(float value);
	void PitchAxisRate(float value);
	void YawAxisRate(float value);
	void JumpActionOn();
	void JumpActionOff();
	void InteractAction();
	void ShadowAction();

	/*MEMBERS*/
private:
	TWeakObjectPtr<AAORBaseCharacter> character;
	AORCharacterMemory memory;
	float timespan;
	bool isRecording;

	FVector beginRecPos;
	FRotator beginRecRot;
};
