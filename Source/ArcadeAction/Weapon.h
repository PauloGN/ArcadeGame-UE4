// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup	 UMETA(DisplayName = "Pickup"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_MAX UMETA(DisplayName = "DefaultMAX"),
};

/**
 * 
 */
UCLASS()
class ARCADEACTION_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | States")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquippeSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
	class USkeletalMeshComponent* SkeletalMesh;



public:


	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	void Equip(class AMainCharacter* Char);
	
	FORCEINLINE void SetWeaponState(EWeaponState EWS){ WeaponState = EWS; }
	FORCEINLINE EWeaponState SetWeaponState(){ return WeaponState; }

};
