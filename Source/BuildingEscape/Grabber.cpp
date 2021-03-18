// Copyright Giancarlo Tomasello 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetUpInputComponent();
	
}

//Check for Physics Handle Component
void UGrabber::FindPhysicsHandle(){
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr){
		UE_LOG(LogTemp, Error, TEXT("%s does not have the Physics Handle Component!"), *GetOwner()->GetName());
	}
}

//Setup and Bind InputComponent
void UGrabber::SetUpInputComponent(){
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent != nullptr){
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab(){

	//Try and reach any actor with physics body collision channel set.
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	//If we hit something then attach the physics handle
	if(ActorHit != nullptr){
		if(PhysicsHandle == nullptr){return;}
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayersReach());
	}
}

void UGrabber::Release(){
	if(PhysicsHandle == nullptr){return;}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If physics handle is attached
	if(PhysicsHandle == nullptr){return;}
	if(PhysicsHandle->GrabbedComponent){
		//Move the object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const{
	/*DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.0f,
		0,
		5.0f
	); */
	
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	// Ray-cast out to a certain distance (Reach)
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	
	AActor* ActorHit = Hit.GetActor();
	//Log out to test

	// if(Hit.Actor != nullptr){
	// 	UE_LOG(LogTemp, Warning, TEXT("The object hit by the line was: %s"), *(ActorHit->GetName()));
	// }
	
	//See what the ray hits
	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetPlayersReach() const{
	// Get Players view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	//Draw a line from player showing the reach.
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

}

