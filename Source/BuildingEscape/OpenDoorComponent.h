// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "OpenDoorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoorComponent();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float GetMassInsideVolume() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	// Setup
	float InitialYaw;
	float DoorLastOpened = 0.f;
	void SetupPointers();
	bool ArePointersAssigned() const;
	
	// Play sound components
	bool IsDoorClosed = true;
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
	void PlaySound();

	
	// Trigger Door Open/Close
	float GetActorsMass(TArray<AActor*> ActorsInPressurePlate) const;
	float GetActorsMassWithTag(TArray<AActor*> ActorsInPressurePlate) const;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY()
	AActor* ActorThatOpens = nullptr;

	// If using the following two, only actor that are properly tagged will open the door
	UPROPERTY(EditAnywhere)
	bool bDoorUseTag = false;

	UPROPERTY(EditAnywhere)
	FName TagThatOpensTheDoor = TEXT("");

	// Component Editable components
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.4f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.8f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.2f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;
};
