// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Timespan.h"

enum class AORCharacterAction {
	JUMP_ON, JUMP_OFF, INTERACT, NONE
};

struct AORMovementRecord 
{
	float forward;
	float sideways;
	FVector position;
	FRotator rotation;
	float timespan;
	AORMovementRecord() {}
	AORMovementRecord(float forward, float sideways, FVector position, FRotator rotation, float timespan)
		: forward(forward), sideways(sideways), position(position), rotation(rotation), timespan(timespan){}
};

struct AORActionEvent
{
	AORCharacterAction action;
	float timespan;
	AORActionEvent() {}
	AORActionEvent(AORCharacterAction action, float timespan)
		: action(action), timespan(timespan) {}
};

struct AORCharacterMemory
{
	TArray<AORMovementRecord> movements;
	TArray<AORActionEvent> actions;
	float timespan;

	void Clear() {
		movements.Empty();
		actions.Empty();
		timespan = 0;
	}

	FString ToString() {
		//transforms
		FString oStr = FString(TEXT("\n***MEMORY***\n\n::MOVEMENTS::\n"));
		for (auto& t : movements) {
			oStr += FString::Printf(TEXT(" -- TIME: %f ==> AXIS: [ %d, %d ] , POS: { %s } , ROT: { %s }\n"),
				t.timespan, t.forward, t.sideways, *t.position.ToString(), *t.rotation.ToString());
		}
		oStr.Append(TEXT("\n"));
		return oStr;
	}
};
