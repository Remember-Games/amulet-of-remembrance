// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Timespan.h"

enum class AORCharacterAction {
	JUMP_ON, JUMP_OFF, INTERACT, NONE
};

struct AORMovementEvent 
{
	float forward;
	float sideways;
	FTimespan timespan;
	AORMovementEvent() {}
	AORMovementEvent(float forward, float sideways, FTimespan timespan)
		: forward(forward), sideways(sideways), timespan(timespan){}
};

struct AORRotationEvent 
{
	float pitch;
	float yaw;
	FTimespan timespan;
	AORRotationEvent() {}
	AORRotationEvent(float pitch, float yaw, FTimespan timespan)
		: pitch(pitch), yaw(yaw), timespan(timespan){}
};

struct AORActionEvent 
{
	AORCharacterAction action;
	FTimespan timespan;
	AORActionEvent() {}
	AORActionEvent(AORCharacterAction action, FTimespan timespan)
		: action(action), timespan(timespan){}
};

struct AORCorrectionEvent
{
	FVector position;
	FRotator rotation;
	FTimespan timespan;
	AORCorrectionEvent() {}
	AORCorrectionEvent(FVector position, FRotator rotation, FTimespan timespan)
		: position(position), rotation(rotation), timespan(timespan){}
};

struct AORCharacterMemory
{
	TArray<AORMovementEvent> movements;
	TArray<AORRotationEvent> rotations;
	TArray<AORActionEvent> actions;
	TArray<AORCorrectionEvent> corrections;
	FTimespan timespan;

	void Clear() {
		movements.Empty();
		rotations.Empty();
		actions.Empty();
		corrections.Empty();
		timespan = FTimespan(0);
	}
};
