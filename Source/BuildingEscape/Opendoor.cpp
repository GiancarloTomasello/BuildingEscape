// Copyright Giancarlo Tomasello 2021

#include "Opendoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

#define OUT

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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();

}

void UOpendoor::FindAudioComponent(){
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(AudioComponent == nullptr){
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpendoor::FindPressurePlate(){
	if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("%s has the DoorRotation component attached, yet no defined PressurePlate."), *GetOwner()->GetActorLabel());
	}
};

// Called every frame
void UOpendoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() >= MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} else {
		//if the dor has been open longer than x seconds
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}	

	}
	
	
}

void UOpendoor::OpenDoor(float DeltaTime){

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	// OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2);

	CloseDoorSound = false;
	if(AudioComponent == nullptr){return;}
	if(!OpenDoorSound){
		AudioComponent->Play();
		OpenDoorSound = true;
	}


}

void UOpendoor::CloseDoor(float DeltaTime){

	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	// OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2);

	OpenDoorSound = false;
	if(AudioComponent == nullptr){return;}
	if(!CloseDoorSound){
		AudioComponent->Play();
		CloseDoorSound = true;
	}
	
}

float UOpendoor::TotalMassOfActors() const{
	float TotalMass = 0.0f;

	//Find all overlapping actors.
	TArray<AActor*> OverlapingActors;
	if(PressurePlate == nullptr) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlapingActors);

	//Add up their masses.
	for(AActor* Actor : OverlapingActors){
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

