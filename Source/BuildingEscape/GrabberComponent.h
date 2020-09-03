// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabberComponent.generated.h"

struct FLineTraceLocation {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector LineTraceEndLocation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Grab();
	void Release();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;
		
	UPhysicsHandleComponent* PhysicsHandler = nullptr;
	UInputComponent* InputComponent = nullptr; // All actor have input component by default.

	void SetupComponents();
	void SetupInputBinding() const;
	FHitResult GetFirstHitBodyInReach() const;
	FLineTraceLocation GetLineTraceLocation() const;
};
