// Copyright Giancarlo Tomasello 2021


#include "Opendoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpendoor::UOpendoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpendoor::BeginPlay()
{
	Super::BeginPlay();

	InitalYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitalYaw;
	TargetYaw += InitalYaw;

		if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("%s has the DoorRotation component attached, yet no defined PressurePlate."), *GetOwner()->GetActorLabel());
	}
}


// Called every frame
void UOpendoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
	}
	
	
}

void UOpendoor::OpenDoor(float DeltaTime){

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * 0.5f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	
	// OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2);

}

