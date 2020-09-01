// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoorComponent::UOpenDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set Door Final Yaw value when opened
	RelativeTargetYaw = GetOwner()->GetActorRotation().Yaw + DoorOpeningDegree;
	
}


// Called every frame
void UOpenDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetOwner()->SetActorRotation(
		FMath::Lerp(								// This function returns a FRotator
			GetOwner()->GetActorRotation(),			// Current Rotation
			FRotator(0.f, RelativeTargetYaw, 0.f),	// Target Rotation
			0.05									// Constant
		)
	);

}

