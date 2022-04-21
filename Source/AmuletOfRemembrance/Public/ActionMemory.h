// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionMemory.generated.h"

struct ActionRecord{
	FName action;
	bool isPress;
	double time;
	ActionRecord(FName action, bool isPress, double time): action(action), isPress(isPress), time(time){}
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AMULETOFREMEMBRANCE_API UActionMemory : public UObject
{
	GENERATED_BODY()
private:
	TArray<ActionRecord> _queue;
public:
	
	UFUNCTION(BlueprintCallable, Category = "Function")
	void RecordAction(FName action, double time, bool isPress);

	UFUNCTION(BlueprintCallable, Category = "Function")
	void Clear();

	UFUNCTION(BlueprintCallable, Category = "Function")
	void DebugPrint();

	TArray<ActionRecord> MakeArray();
};
