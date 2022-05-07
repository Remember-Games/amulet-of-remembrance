// Fill out your copyright notice in the Description page of Project Settings.


#include "AORShadowController.h"
#include "Math/UnrealMathUtility.h"

AAORShadowController::AAORShadowController() 
{
	beginMemTime = FDateTime::UtcNow();
	mInd = 0;
	aInd = 0;
	isPositionMatched = true;
}

void AAORShadowController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);
	character = Cast<AAORBaseCharacter>(inPawn);
	checkf(character, TEXT("Shadow pawn needs to be a subclass of AORBaseCharacter!"));
}

void AAORShadowController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	FTimespan tspan = FDateTime::UtcNow() - beginMemTime;
	if (tspan > memory.timespan) {
		character->Destroy();
		return; 
	}
	DoMovements(tspan);
	DoActions(tspan);
}

void AAORShadowController::SetNewMemory(AORCharacterMemory& _memory)
{
	memory = std::move(_memory);
	beginMemTime = FDateTime::UtcNow();
}

void AAORShadowController::DoMovements(FTimespan ts)
{
	if (mInd >= memory.movements.Num() || ts < memory.movements[mInd].timespan)return;
	while (mInd + 1 < memory.movements.Num() &&
		memory.movements[mInd + 1].timespan < ts)++mInd;
	AORMovementRecord tr_from = memory.movements[mInd];
	FVector targetPos;
	FRotator targetRot;
	float forward;
	float sideways;
	if (mInd + 1 < memory.movements.Num()) {
		AORMovementRecord tr_to = memory.movements[mInd + 1];
		double l = (tr_to.timespan - tr_from.timespan).GetTotalSeconds();
		double delta = (ts - tr_from.timespan).GetTotalSeconds();
		double k = l != 0 ? delta / l : delta;
		targetPos = FMath::Lerp(tr_from.position, tr_to.position, k);
		targetRot = FMath::Lerp(tr_from.rotation, tr_to.rotation, k);
		forward = FMath::Lerp(tr_from.forward, tr_to.forward, k);
		sideways = FMath::Lerp(tr_from.sideways, tr_to.sideways, k);
	}
	else {
		targetPos = tr_from.position;
		targetRot = tr_from.rotation;
		forward = tr_from.forward;
		sideways = tr_from.sideways;
		mInd++;
	}
	SetControlRotation(targetRot);
	if (isPositionMatched) {
		FVector vec = targetPos - character->GetActorLocation();
		if (vec.Length() < MAX_CORRECTION_POS_MAG) {
			character->SetActorLocation(targetPos, true);
			UE_LOG(LogTemp, Warning, TEXT("POSITION MATCHED!"));
		}
		else {
			isPositionMatched = false;
			UE_LOG(LogTemp, Warning, TEXT("POSITION DEATACHED!"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("POSITION DEATACHED!"));
	}
	const FRotator yawRotation(0, GetControlRotation().Yaw, 0);
	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector sidewaysDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	character->AddMovementInput(forwardDirection, forward);
	character->AddMovementInput(sidewaysDirection, sideways);
}

void AAORShadowController::DoActions(FTimespan ts)
{
	AORActionEvent e;
	while (aInd < memory.actions.Num() && (e = memory.actions[aInd]).timespan < ts) {
		switch (e.action) {
		case AORCharacterAction::JUMP_ON:
			character->Jump();
			break;
		case AORCharacterAction::JUMP_OFF:
			character->StopJumping();
			break;
		case AORCharacterAction::INTERACT:
			character->Interact();
			break;
		}
		aInd++;
	}
}

