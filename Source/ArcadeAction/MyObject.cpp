// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject.h"

UMyObject::UMyObject():myFloat(0.0f)
{
}

void UMyObject::MyFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("My Float %f"),myFloat);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Testing print on screen "));
	}

}
