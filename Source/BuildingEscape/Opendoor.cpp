// Copyright Giancarlo Tomasello 2021


#include "Opendoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpendoor::UOpendoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpendoor::BeginPlay()
{
	Super::BeginPlay();

	float MyFloat = 10.0f;

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = 90.0f;
	GetOwner()->SetActorRotation(CurrentRotation);
}


// Called every frame
void UOpendoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

