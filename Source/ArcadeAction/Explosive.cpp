// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AExplosive::AExplosive():Damage(10.f)
{
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "AExplosive::Begin Overlap");

	if (OtherActor)
	{
		AMainCharacter* MainCharREF = Cast<AMainCharacter>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);

		if (MainCharREF || Enemy)
		{
			//MainCharREF->DecrementHealth(Damage);
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);

			if (OverlapParticle != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator().ZeroRotator, true);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			Destroy();
		}

	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "AExplosive::End Overlap");


}
