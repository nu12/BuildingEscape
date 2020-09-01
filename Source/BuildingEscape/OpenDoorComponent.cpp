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
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw += InitialYaw;
	
}


// Called every frame
void UOpenDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Interpolation options
	// FMath::Lerp -> Not plataform independent						===> FMath::Lerp(CurrentRotation, TargetRotation, 0.05); GetOwner()->SetActorRotation(FMath::Lerp(CurrentRotation, TargetRotation, 0.05));
	// FMath::FInterpTo -> Same exponential interpolation behavior	===> FMath::FInterpTo(CurrentRotation.Yaw, RelativeTargetYaw, DeltaTime, 2);GetOwner()->SetActorRotation(FRotator(0.f, NextYawPosition,0.f));
	// FMath::FInterpConstantTo -> Linear interpolation behavior	===> FMath::FInterpConstantTo(CurrentRotation.Yaw, RelativeTargetYaw, DeltaTime, 45);GetOwner()->SetActorRotation(FRotator(0.f, NextYawPosition,0.f));

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	FRotator TargetRotation = FRotator(0.f, TargetYaw, 0.f);
	float NextYawPosition = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw, DeltaTime, 2);
	
	GetOwner()->SetActorRotation(FRotator(0.f, NextYawPosition,0.f));
}

