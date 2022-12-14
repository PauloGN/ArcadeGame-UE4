// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "MainCharacter.h"


APickup::APickup()
{

}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "APickup::Begin Overlap");

	if (OtherActor)
	{
		AMainCharacter* MainCharREF = Cast<AMainCharacter>(OtherActor);

		if (MainCharREF)
		{
			OnPickupBP(MainCharREF);
			MainCharREF->PickUpLocations.Add(GetActorLocation());
			Destroy();
		}
	}
}

void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "APickup::End Overlap");

}
