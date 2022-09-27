// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class ARCADEACTION_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

	/** Trigger Volume for the functionalies*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Triggers")
	class UBoxComponent* TriggerBox;

	/** Mesh for the character step on*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Triggers")
	class UStaticMeshComponent* FloorSwitch;

	/** Door/Gate to move/open when the character step on*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Triggers")
	UStaticMeshComponent* Door;

	/** Initial Location for the door */
	UPROPERTY(BlueprintReadWrite, Category = "Triggers")
	FVector InitialDoorLocation;

	/** Initial Location for the floorSwitch */
	UPROPERTY(BlueprintReadWrite, Category = "Triggers")
	FVector InitialFloorSwitchLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//** Implementation via Blueprints */
	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
	void RaiseDoor();
	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
	void RaiseFloorSwitch();
	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
	void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable, Category = "FloorSwitch")
	void UpdateDoorLocationInZ(float z);


	UFUNCTION(BlueprintCallable, Category = "FloorSwitch")
	void UpdateFloorLocationInZ(float z);
};
