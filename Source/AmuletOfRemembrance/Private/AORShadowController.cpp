// Fill out your copyright notice in the Description page of Project Settings.


#include "AORShadowController.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

AAORShadowController::AAORShadowController() 
{
	timespan = 0;
	mInd = 0;
	aInd = 0;
	isCorrectionForce = true;
	lastTargetPos = FVector(0, 0, 0);

	Spring = 150;
	Damper = 0.2;
	BreakDistance = 200;
	MaxForceDistance = 200;
}

void AAORShadowController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);
	character = Cast<AAORBaseCharacter>(inPawn);
	checkf(character != nullptr, TEXT("Shadow pawn needs to be a subclass of AORBaseCharacter!"));
	characterMovement = character->GetCharacterMovement();
	lastTargetPos = character->GetActorLocation();
}

void AAORShadowController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	timespan += deltaTime;
	if (timespan > memory.timespan) {
		character->Destroy();
		return; 
	}
	DoMovements(timespan, deltaTime);
	DoActions(timespan);
}

void AAORShadowController::SetNewMemory(AORCharacterMemory& _memory)
{
	memory = std::move(_memory);
	timespan = 0;
}

void AAORShadowController::DoMovements(float ts, float deltaTime)
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
		float l = tr_to.timespan - tr_from.timespan;
		float delta = ts - tr_from.timespan;
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
	FVector targetVel = (targetPos - lastTargetPos) / deltaTime;
	lastTargetPos = targetPos;
	SetControlRotation(targetRot);
	ApplyCorrectionForce(targetPos, targetVel, deltaTime);
	
	const FRotator yawRotation(0, GetControlRotation().Yaw, 0);
	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector sidewaysDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	character->AddMovementInput(forwardDirection, forward);
	character->AddMovementInput(sidewaysDirection, sideways);
}

void AAORShadowController::DoActions(float ts)
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

void AAORShadowController::ApplyCorrectionForce(FVector targetPos, FVector targetVel, float deltaTime)
{
	DrawDebugSphere(GetWorld(), targetPos, 10, 8, FColor::Green, true, 0, 1, 1);
	DrawDebugLine(GetWorld(), character->GetActorLocation(), targetPos, FColor::Red, true, 0, 1, 1);
	
	if (isCorrectionForce) {
		FVector deltaPos = targetPos - character->GetActorLocation();
		FVector deltaVel = targetVel - characterMovement->Velocity;
		UE_LOG(LogTemp, Warning, TEXT("DeltaPos: %s"), *deltaPos.ToString());
		UE_LOG(LogTemp, Warning, TEXT("DeltaVel: %s | TargetVel: %s | ChVel: %s"), *deltaVel.ToString(), *targetVel.ToString(), *characterMovement->Velocity.ToString());
		if (deltaPos.Length() < BreakDistance) {
			FVector springForce = (Spring * deltaPos).GetClampedToMaxSize(Spring * MaxForceDistance); 
			FVector damperForce = (Damper * Spring * deltaVel).GetClampedToMaxSize(Damper * Spring * MaxForceDistance);
			//FMath::Clamp(springForce, )
			FVector correctionForce = (springForce + damperForce)*characterMovement->Mass;
			characterMovement->AddForce(correctionForce);
			UE_LOG(LogTemp, Warning, TEXT("Spring Force: %s | Damping Force: %s"), *springForce.ToString(), *damperForce.ToString());
		}
		else {
			isCorrectionForce = false;
		}
	}
}

