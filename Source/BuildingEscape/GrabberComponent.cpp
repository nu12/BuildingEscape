// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GrabberComponent.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	FVector LineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineEnd, FColor::Red, false, 1.f, 1, 10.f);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		PlayerViewPointLocation,
		LineEnd,
		ECollisionChannel::ECC_PhysicsBody,
		FCollisionQueryParams(
			FName(TEXT("")),	// This is not using Tag
			false,				// No complex collision (visibility vs collision)
			GetOwner()			// Ignore self as the line tracing begins inside the pawn
		)
	);
	
	if (!Hit.GetActor()) return;
	UE_LOG(LogTemp, Warning, TEXT("Hit: %f %s"), GetWorld()->GetTimeSeconds(), *Hit.GetActor()->GetName());
}

