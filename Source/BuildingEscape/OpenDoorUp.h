// Copyright Giancarlo Tomasello 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoorUp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoorUp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoorUp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;
		
private:

	FVector startingLocation;
	FVector endLocation;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 60.0f;

	UPROPERTY(EditAnywhere)
	float slidingLength = 250.0f;
	
	float DoorLastOpened = 0.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.0f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 0.8f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent = nullptr;
};
