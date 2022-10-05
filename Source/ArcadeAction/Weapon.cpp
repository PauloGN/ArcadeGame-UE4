// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GamePlayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AWeapon::AWeapon():OnEquippeSound(nullptr)
{

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor)
	{
		AMainCharacter* CharREF = Cast<AMainCharacter>(OtherActor);

		if (CharREF)
		{

			CharREF->SetActiveOverlappingItem(this);
			//Equip(CharREF);
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		AMainCharacter* CharREF = Cast<AMainCharacter>(OtherActor);

		if (CharREF)
		{

			CharREF->SetActiveOverlappingItem(nullptr);
			//Equip(CharREF);
		}
	}
}

void AWeapon::Equip(AMainCharacter* Char)
{
	if (Char)
	{
		//Dont Get the Camera zooming in if the weapon is in the way
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName(TEXT("RightHandSocket"));
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, Char->GetMesh());
			bSpinning = false;
			//Set the weapon ref to the char
			Char->SetEquippedWeapon(this);
		}
		//Play sound effect
		if (OnEquippeSound)
		{
			UGameplayStatics::PlaySound2D(this, OnEquippeSound);
			IdleParticleComponent->Deactivate();

		}
	}
}
