// Fill out your copyright notice in the Description page of Project Settings.


#include "AORShadowController.h"

AAORShadowController::AAORShadowController() 
{
	beginMemTime = FDateTime::UtcNow();
	movementsInd = 0;
	rotationsInd = 0;
	actionsInd = 0;
	correctionsInd = 0;
	isCorrected = true;
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
		Destroy();
		return;
	}
	//DoCorrections(tspan);
	DoMovements(tspan);
	DoRotations(tspan);
	DoActions(tspan);
}

void AAORShadowController::SetNewMemory(AORCharacterMemory& _memory)
{
	memory = std::move(_memory);
	beginMemTime = FDateTime::UtcNow();
}

void AAORShadowController::DoMovements(FTimespan ts)
{
	AORMovementEvent e;
	while (movementsInd < memory.movements.Num() && (e = memory.movements[movementsInd]).timespan < ts) {
		const FRotator yawRotation(0, GetControlRotation().Yaw, 0);
		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		const FVector sidewaysDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		character->AddMovementInput(forwardDirection, e.forward);
		character->AddMovementInput(sidewaysDirection, e.sideways);
		movementsInd++;
	}
}

void AAORShadowController::DoRotations(FTimespan ts)
{
	AORRotationEvent e;
	while (rotationsInd < memory.rotations.Num() && (e = memory.rotations[rotationsInd]).timespan < ts) {
		character->AddControllerPitchInput(e.pitch);
		character->AddControllerYawInput(e.yaw);
		rotationsInd++;
	}
}

void AAORShadowController::DoActions(FTimespan ts)
{
	AORActionEvent e;
	while (actionsInd < memory.actions.Num() && (e = memory.actions[actionsInd]).timespan < ts) {
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
		actionsInd++;
	}
}

void AAORShadowController::DoCorrections(FTimespan ts)
{
	if (!isCorrected)return;
	AORCorrectionEvent e;
	while (correctionsInd < memory.corrections.Num() && (e = memory.corrections[correctionsInd]).timespan < ts) {
		FVector currentPos = character->GetActorLocation();
		FRotator currentRot = character->GetActorRotation();
		FVector targetPos = e.position;
		FRotator targetRot = e.rotation;

		FVector deltaPos = targetPos - currentPos;
		if (deltaPos.Length() < MAX_CORRECTION_POS_MAG) {
			character->SetActorLocation(targetPos, true);
		}
		else {
			isCorrected = false;
		}
		character->SetActorRotation(targetRot);
		correctionsInd++;
	}
}