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

}

// Called to bind functionality to input
void AAORBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

