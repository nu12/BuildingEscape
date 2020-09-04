// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "OpenDoorComponent.h"

// Sets default values for this component's properties
UOpenDoorComponent::UOpenDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UOpenDoorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set Door Final Yaw value when opened
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += InitialYaw;

	SetupPointers();
}


void UOpenDoorComponent::SetupPointers() {
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ArePointersAssigned()) return;
	
	if (GetMassInsideVolume() >= MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}else
	{
		if(GetWorld()->GetTimeSeconds() > (DoorLastOpened + DoorCloseDelay))
		{
			CloseDoor(DeltaTime);
		}
	}
}

bool UOpenDoorComponent::ArePointersAssigned() const{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenDoorComponent needs assigned PressurePlate for %s"), *GetOwner()->GetName());
		return false;
	}

	if (!ActorThatOpens)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenDoorComponent needs assigned ActorThatOpens for %s"), *GetOwner()->GetName());
		return false;
	}
	return true;
}

float UOpenDoorComponent::GetMassInsideVolume() const {
	TArray<AActor*> ActorsInPressurePlate;
	PressurePlate->GetOverlappingActors(ActorsInPressurePlate);

	if (bDoorUseTag) return GetActorsMassWithTag(ActorsInPressurePlate);
	return GetActorsMass(ActorsInPressurePlate);
}
float UOpenDoorComponent::GetActorsMass(TArray<AActor*> ActorsInPressurePlate) const {
	float TotalMass = 0.f;
	for (AActor* ActorInPressurePlate : ActorsInPressurePlate) {
		TotalMass += ActorInPressurePlate->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

float UOpenDoorComponent::GetActorsMassWithTag(TArray<AActor*> ActorsInPressurePlate) const {
	float TotalMass = 0.f;
	for (AActor* ActorInPressurePlate : ActorsInPressurePlate) {
		for (FName Tag : ActorInPressurePlate->Tags) {
			if (Tag.IsEqual(TagThatOpensTheDoor)) {
				TotalMass += ActorInPressurePlate->FindComponentByClass<UPrimitiveComponent>()->GetMass();
				break;
			}
		}
	}
	return TotalMass;
}

/* Interpolation options
 FMath::Lerp -> Not plataform independent						===> FMath::Lerp(CurrentRotation, TargetRotation, 0.05); GetOwner()->SetActorRotation(FMath::Lerp(CurrentRotation, TargetRotation, 0.05));
 FMath::FInterpTo -> Same exponential interpolation behavior	===> FMath::FInterpTo(CurrentRotation.Yaw, RelativeTargetYaw, DeltaTime, 2);GetOwner()->SetActorRotation(FRotator(0.f, NextYawPosition,0.f));
 FMath::FInterpConstantTo -> Linear interpolation behavior		===> FMath::FInterpConstantTo(CurrentRotation.Yaw, RelativeTargetYaw, DeltaTime, 45);GetOwner()->SetActorRotation(FRotator(0.f, NextYawPosition,0.f)); */
void UOpenDoorComponent::OpenDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	FRotator TargetRotation = FRotator(0.f, OpenAngle, 0.f);
	float NextYawPosition = FMath::FInterpTo(CurrentRotation.Yaw, OpenAngle, DeltaTime, DoorOpenSpeed);

	GetOwner()->SetActorRotation(FRotator(0.f, NextYawPosition, 0.f));
	
}

void UOpenDoorComponent::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	FRotator TargetRotation = FRotator(0.f, InitialYaw, 0.f);
	float NextYawPosition = FMath::FInterpTo(CurrentRotation.Yaw, InitialYaw, DeltaTime, DoorCloseSpeed);

	GetOwner()->SetActorRotation(FRotator(0.f, NextYawPosition, 0.f));
}