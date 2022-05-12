 // Fill out your copyright notice in the Description page of Project Settings.


#include "AORBaseCharacter.h"
#include "Camera/CameraComponent.h"


// Sets default values
AAORBaseCharacter::AAORBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	interactFocus = nullptr;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("POVCamera"));
	FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->SetupAttachment(GetMesh(), "Head");
}

// Called when the game starts or when spawned
void AAORBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAORBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Player Character TICK: %u"), FDateTime::UtcNow().GetTicks());
	UpdateInteractionFocus();
}

void AAORBaseCharacter::Interact()
{
	if (interactFocus) {
		interactFocus->Interact();
	}
}

void AAORBaseCharacter::BeginShadowRecord()
{
	
}

void AAORBaseCharacter::StopShadowRecord()
{

}

void AAORBaseCharacter::UpdateInteractionFocus()
{
	FVector interactOrigin = GetActorLocation() + FVector(0, 0, InteractOriginHeight);
	FVector interactEnd = GetControlRotation().Vector() * InteractReach;
	FHitResult out;
	if (GetWorld()->LineTraceSingleByChannel(out, interactOrigin, interactEnd, ECollisionChannel::ECC_Visibility)) {
		auto* interactive = Cast<IAORInteractive>(out.GetActor());
		if (interactive && interactive->CanInteract()) {
			if (interactive != interactFocus) {
				interactive->HighlightOn();
				if (interactFocus)interactFocus->HighlightOff();
			}
			interactFocus = interactive;
		}
		else if(interactFocus){
			interactFocus->HighlightOff();
			interactFocus = nullptr;
		}
	}
	else if(interactFocus){
		interactFocus->HighlightOff();
		interactFocus = nullptr;
	}
}

