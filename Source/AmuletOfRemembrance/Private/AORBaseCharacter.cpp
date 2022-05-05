// Fill out your copyright notice in the Description page of Project Settings.


#include "AORBaseCharacter.h"

// Sets default values
AAORBaseCharacter::AAORBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
}

void AAORBaseCharacter::Interact()
{

}

void AAORBaseCharacter::BeginShadowRecord()
{

}

void AAORBaseCharacter::StopShadowRecord()
{

}

