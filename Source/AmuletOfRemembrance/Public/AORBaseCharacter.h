// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "AORInteractive.h"
#include "UObject/WeakInterfacePtr.h"
#include "AORBaseCharacter.generated.h"

UCLASS()
class AMULETOFREMEMBRANCE_API AAORBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float InteractOriginHeight = 170;

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float InteractReach = 100;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	// Sets default values for this character's properties
	AAORBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact();
	virtual void BeginShadowRecord();
	virtual void StopShadowRecord();

private:
	void UpdateInteractionFocus();

private:
	TWeakInterfacePtr<IAORInteractive> interactFocus;
};
