// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class ARCADEACTION_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	//this is a mesh used as a platform
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	class UStaticMeshComponent* PlatformMesh;
	
	UPROPERTY(EditAnyWhere, Category = "Platform")
	FVector StartPoint;//Placed location

	UPROPERTY(EditAnyWhere, Category = "Platform", meta = (MakeEditWidget = "true"))
	FVector EndPoint;//Target location relative to the Startpoint, not relative to the world location.

	/** Controls how fast the platform travels from the startPoint to the endpoint */
	UPROPERTY(EditAnyWhere, Category = "Platform")
	float InterpolationSpeed;

	//Tells how many times should the platform wait till moves forward or backward
	UPROPERTY(EditAnyWhere, Category = "Platform")
	float PlatformStopTime;

	//True means go ahead - false wait for a few seconds based on Platform Time
	UPROPERTY(EditAnyWhere, Category = "Platform")
	bool bInterping;

private:

	//cash the total distance from the origin to the target destination of the platform
	float Distance;

	//Use to toggle on and off whether or not we're interpolating
	FTimerHandle PlatformTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Toggle the member variable bIsterping value.
	void ToggleInterping();

	//used to Swap the vectors startPoint to endpoint and vice versa.
	void SwapVector(FVector& V1, FVector& V2);
};
