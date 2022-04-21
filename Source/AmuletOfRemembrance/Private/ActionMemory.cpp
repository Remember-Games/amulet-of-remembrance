// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionMemory.h"

void UActionMemory::RecordAction(FName action, double time, bool isPress) 
{
	_queue.Add(ActionRecord(action, isPress, time));
}

void UActionMemory::Clear()
{
	_queue.Empty();
}

void UActionMemory::DebugPrint()
{
	FString JoinedStr;
	JoinedStr += "Action Memory Array = [\n";
	for (auto& ar : _queue) {
		const FString str = FString::Printf(TEXT("action (%s, %d, %f) \n"), *ar.action.ToString(), ar.isPress, ar.time);
		JoinedStr += str;
	}
	JoinedStr += "\n";
	UE_LOG(LogTemp, Display, TEXT("%s"), *JoinedStr);
}
