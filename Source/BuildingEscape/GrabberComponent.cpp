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
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupComponents();
	SetupInputBinding();
}

void UGrabberComponent::SetupComponents() {
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandler) UE_LOG(LogTemp, Error, TEXT("Physics handler component not found!"));

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) UE_LOG(LogTemp, Error, TEXT("Input component not found!"));
}
void UGrabberComponent::SetupInputBinding() const {
	InputComponent->BindAction(
		FName(TEXT("GrabInput")),	// This is the same name assigned to the input in Project Settings > Input
		IE_Pressed,					// Type of input: pressed, release, etc
		this,						// References self
		&UGrabberComponent::Grab	// Method to be executed
	);

	InputComponent->BindAction(
		FName(TEXT("GrabInput")),
		IE_Released,
		this,
		&UGrabberComponent::Release
	);
}

// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandler->GetGrabbedComponent()) {
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		FVector LineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
		PhysicsHandler->SetTargetLocation(LineEnd);
	}
}

void UGrabberComponent::Grab() 
{
	UE_LOG(LogTemp, Warning, TEXT("Grab!"));
	FHitResult HitResult = GetFirstHitBodyInReach();

	if (!HitResult.GetActor()) return;

	PhysicsHandler->GrabComponentAtLocation(
		HitResult.GetComponent(),
		NAME_None,
		HitResult.GetActor()->GetActorLocation()
	);
}

void UGrabberComponent::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release!"));
	PhysicsHandler->ReleaseComponent();
}

FHitResult UGrabberComponent::GetFirstHitBodyInReach() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	FVector LineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

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
	
	if (!Hit.GetActor()) return Hit;
	UE_LOG(LogTemp, Warning, TEXT("Hit: %f %s"), GetWorld()->GetTimeSeconds(), *Hit.GetActor()->GetName());

	return Hit;
}