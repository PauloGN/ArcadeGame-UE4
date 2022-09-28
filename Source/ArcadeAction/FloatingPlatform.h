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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	class UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnyWhere, Category = "Platform")
	FVector StartPoint;

	UPROPERTY(EditAnyWhere, Category = "Platform", meta = (MakeEditWidget = "true"))
	FVector EndPoint;

	UPROPERTY(EditAnyWhere, Category = "Platform")
	float PlatformSpeed;

	UPROPERTY(EditAnyWhere, Category = "Platform")
	float PlatformTime;

	float Distance;

	FTimerHandle PlatformTimer;
	
	UPROPERTY(EditAnyWhere, Category = "Platform")
	bool bInterping;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void ToggleInterping();

	void SwapVector(FVector& V1, FVector& V2);
};
