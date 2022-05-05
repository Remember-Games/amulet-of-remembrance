// Fill out your copyright notice in the Description page of Project Settings.


#include "AORPlayerController.h"
#include "AORShadowController.h"

AAORPlayerController::AAORPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	isRecording = false;
	character = nullptr;
	MaxRecordingDuration = 10.0f;
	MaxActiveShadows = 1;
	TurnRateGamepad = 50.0f;
}

void AAORPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAxis("Move Forward / Backward", this, &AAORPlayerController::ForwardAxis);
	InputComponent->BindAxis("Move Right / Left", this, &AAORPlayerController::SidewaysAxis);

	InputComponent->BindAxis("Turn Right / Left Mouse", this, &AAORPlayerController::YawAxisDelta);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AAORPlayerController::PitchAxisDelta);
	
	InputComponent->BindAxis("Turn Right / Left Gamepad", this, &AAORPlayerController::YawAxisRate);
	InputComponent->BindAxis("Look Up / Down Gamepad", this, &AAORPlayerController::PitchAxisRate);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AAORPlayerController::JumpActionOn);
	InputComponent->BindAction("Jump", IE_Released, this, &AAORPlayerController::JumpActionOff);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AAORPlayerController::InteractAction);
	InputComponent->BindAction("Shadow Record", IE_Pressed, this, &AAORPlayerController::ShadowAction);
}

void AAORPlayerController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);
	character = Cast<AAORBaseCharacter>(inPawn);
	checkf(character, TEXT("Player pawn needs to be a subclass of AORBaseCharacter!"));
}

void AAORPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	FDateTime now = FDateTime::UtcNow();
	//UE_LOG(LogTemp, Warning, TEXT("Player Controller TICK: %u"), now.GetTicks());
	//DebugPrintCorrection(character->GetActorLocation(), character->GetActorRotation(), now);
	if (isRecording) {
		RecordAxisInputs(deltaTime);
	}
}

void AAORPlayerController::RecordAxisInputs(float deltaTime)
{
	FDateTime now = FDateTime::UtcNow();
	FTimespan timespan = now - beginRecTime;
	if (timespan.GetTotalSeconds() > MaxRecordingDuration) {
		StopShadowRecord();
		return;
	}
	float forwardAxis = InputComponent->GetAxisValue("Move Forward / Backward");
	float sidewaysAxis = InputComponent->GetAxisValue("Move Right / Left");
	float pitchDelta = InputComponent->GetAxisValue("Look Up / Down Mouse");
	float yawDelta = InputComponent->GetAxisValue("Turn Right / Left Mouse");
	float pitchRate = InputComponent->GetAxisValue("Look Up / Down Gamepad");
	float yawRate = InputComponent->GetAxisValue("Turn Right / Left Gamepad");
	if (forwardAxis != 0.0f || sidewaysAxis != 0.0f) {
		memory.movements.Add(AORMovementEvent(forwardAxis, sidewaysAxis, timespan));
	}
	if (pitchDelta != 0.0f || yawDelta != 0.0f) {
		memory.rotations.Add(AORRotationEvent(pitchDelta, yawDelta, timespan));
	}
	if (pitchRate != 0.0f || yawRate != 0.0f) {
		memory.rotations.Add(AORRotationEvent(pitchRate * deltaTime, yawRate * deltaTime, timespan));
	}
	memory.corrections.Add(AORCorrectionEvent(character->GetActorLocation(), character->GetActorRotation(), timespan));
	/*
	UE_LOG(LogTemp, Warning, TEXT("Frame: %u"), now.GetTicks());
	UE_LOG(LogTemp, Warning, TEXT("***Forward: %f"), forwardAxis);
	UE_LOG(LogTemp, Warning, TEXT("***Sideways: %f"), sidewaysAxis);
	UE_LOG(LogTemp, Warning, TEXT("***Pitch: %f"), pitchDelta);
	UE_LOG(LogTemp, Warning, TEXT("***Yaw: %f\n"), yawDelta);
	*/
}

void AAORPlayerController::BeginShadowRecord()
{
	character->BeginShadowRecord();
	isRecording = true;
	beginRecTime = FDateTime::UtcNow();
	beginRecPos = character->GetActorLocation();
	beginRecRot = character->GetActorRotation();
}

void AAORPlayerController::StopShadowRecord()
{
	character->StopShadowRecord();
	isRecording = false;
	FDateTime endRecTime = FDateTime::UtcNow();
	memory.timespan = endRecTime - beginRecTime;
	DebugPrintMemory();
	checkf(ShadowCharacter, TEXT("Shadow Character type class is not set properly!"));
	APawn* shadow_pawn = GetWorld()->SpawnActor<APawn>(ShadowCharacter, beginRecPos, beginRecRot);
	if (shadow_pawn) {
		//shadow_pawn->SpawnDefaultController();
		AAORShadowController* sc = Cast<AAORShadowController>(shadow_pawn->Controller);
		checkf(sc, TEXT("Shadow character controller is not of type AORShadowController!"));
		sc->SetNewMemory(memory);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Shadow actor could not be spawned"));
	}
}

void AAORPlayerController::ForwardAxis(float value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Event: %u"), FDateTime::UtcNow().GetTicks());
	//UE_LOG(LogTemp, Warning, TEXT("***Forward: %f\n"), value);
	if (value != 0.0f) {
		const FRotator yawRotation(0, GetControlRotation().Yaw, 0);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		character->AddMovementInput(direction, value);
	}
}

void AAORPlayerController::SidewaysAxis(float value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Event: %u"), FDateTime::UtcNow().GetTicks());
	//UE_LOG(LogTemp, Warning, TEXT("***Sideways: %f\n"), value);
	if (value != 0.0f) {
		const FRotator yawRotation(0, GetControlRotation().Yaw, 0);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		character->AddMovementInput(direction, value);
	}
}

void AAORPlayerController::PitchAxisDelta(float value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Event: %u"), FDateTime::UtcNow().GetTicks());
	//UE_LOG(LogTemp, Warning, TEXT("***Pitch: %f\n"), value);
	character->AddControllerPitchInput(value);
}

void AAORPlayerController::YawAxisDelta(float value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Event: %u"), FDateTime::UtcNow().GetTicks());
	//UE_LOG(LogTemp, Warning, TEXT("***Yaw: %f\n"), value);
	character->AddControllerYawInput(value);
}

void AAORPlayerController::PitchAxisRate(float value)
{
	character->AddControllerPitchInput(value * GetWorld()->GetDeltaSeconds());
}

void AAORPlayerController::YawAxisRate(float value)
{
	character->AddControllerYawInput(value * GetWorld()->GetDeltaSeconds());
}

void AAORPlayerController::JumpActionOn()
{
	//UE_LOG(LogTemp, Warning, TEXT("Event: %u"), FDateTime::UtcNow().GetTicks());
	//UE_LOG(LogTemp, Warning, TEXT("***Jump On\n"));
	character->Jump();
	if (isRecording) {
		FTimespan timespan = FDateTime::UtcNow() - beginRecTime;
		memory.actions.Add(AORActionEvent(AORCharacterAction::JUMP_ON, timespan));
	}
}

void AAORPlayerController::JumpActionOff()
{
	//UE_LOG(LogTemp, Warning, TEXT("Event: %u"), FDateTime::UtcNow().GetTicks());
	//UE_LOG(LogTemp, Warning, TEXT("***Jump Off\n"));
	character->StopJumping();
	if (isRecording) {
		FTimespan timespan = FDateTime::UtcNow() - beginRecTime;
		memory.actions.Add(AORActionEvent(AORCharacterAction::JUMP_OFF, timespan));
	}
}

void AAORPlayerController::InteractAction()
{
	character->Interact();
	if (isRecording) {
		FTimespan timespan = FDateTime::UtcNow() - beginRecTime;
		memory.actions.Add(AORActionEvent(AORCharacterAction::INTERACT, timespan));
	}
}

void AAORPlayerController::ShadowAction()
{
	if (!isRecording)BeginShadowRecord();
	else StopShadowRecord();
}


void AAORPlayerController::DebugPrintMemory()
{

}

void AAORPlayerController::DebugPrintCorrection(FVector pos, FRotator rot, FDateTime t)
{
	UE_LOG(LogTemp, Warning, TEXT("Correction Event ::: Pos : [ %s ] | Rot : [ %s ] | time : [ %u ]"), *pos.ToString(), *rot.ToString(), t.GetTicks());
}
