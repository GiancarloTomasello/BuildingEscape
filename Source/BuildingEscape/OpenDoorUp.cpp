// Copyright Giancarlo Tomasello 2021


#include "OpenDoorUp.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoorUp::UOpenDoorUp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorUp::BeginPlay()
{
	Super::BeginPlay();


	startingLocation = GetOwner()->GetActorLocation();
	endLocation = startingLocation + FVector(0.0f, 0.0f, slidingLength);

	FindPressurePlate();
	FindAudioComponent();
	
}

void UOpenDoorUp::FindAudioComponent(){
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(AudioComponent == nullptr){
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoorUp::FindPressurePlate(){
	if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("%s has the DoorRotation component attached, yet no defined PressurePlate."), *GetOwner()->GetActorLabel());
	}
};

// Called every frame
void UOpenDoorUp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UOpenDoorUp::OpenDoor(float DeltaTime){

	float currentZ = GetOwner()->GetActorLocation().Z;
	FVector OpenDoor = GetOwner()->GetActorLocation();
	OpenDoor.Z = FMath::FInterpTo(currentZ, endLocation.Z, DeltaTime,1);
	GetOwner()->SetActorLocation(OpenDoor);
}

void UOpenDoorUp::CloseDoor(float DeltaTime){

	float currentZ = GetOwner()->GetActorLocation().Z;
	FVector OpenDoor = GetOwner()->GetActorLocation();
	OpenDoor.Z = FMath::FInterpTo(currentZ, startingLocation.Z, DeltaTime,1);
	GetOwner()->SetActorLocation(OpenDoor);
}
float UOpenDoorUp::TotalMassOfActors() const{
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

