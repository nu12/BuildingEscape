// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPositionComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPositionComponent::UWorldPositionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPositionComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("World Position Component attached to %s, position %s"), 
		*GetOwner()->GetName(),
		*GetOwner()->GetActorLocation().ToCompactString()
	);
	

}


// Called every frame
void UWorldPositionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

