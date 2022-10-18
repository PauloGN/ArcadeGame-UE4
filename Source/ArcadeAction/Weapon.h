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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	class UBoxComponent* CombatBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SwingSound;

	//Apply Damage params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	//Going to use the mainChar controller here
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;

	class AMainCharacter* CharREFCauser;

protected:

	virtual void BeginPlay()override;

public:

	/** Operlapping Functions */
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UFUNCTION()
	void OnCombateBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void Equip(AMainCharacter* Char);
	
	FORCEINLINE void SetWeaponState(EWeaponState EWS){ WeaponState = EWS; }
	FORCEINLINE EWeaponState SetWeaponState(){ return WeaponState; }
	//Weap apply dmg
	FORCEINLINE void SetInstigator(AController* Inst){ WeaponInstigator = Inst; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ActivateCollision();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable, Category = "Combat | Sound")
	void PlaySwingSound();

};
