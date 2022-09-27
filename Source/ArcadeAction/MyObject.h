// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObject.generated.h"

/**
 * Blueprintable key word inside the UClass macro allows us to create a blue print of the type MyObject into the unreal editor
 * this is a basic clas not designed to be placed into the world however it can holds variables and functions that can be useful when it comes to game mecanics 
 * 
 */
UCLASS(Blueprintable)
class ARCADEACTION_API UMyObject : public UObject
{
	GENERATED_BODY()

public:

	UMyObject();

	UPROPERTY(BlueprintReadWrite, Category = "My Variables")
	float myFloat;

	UFUNCTION(BlueprintCallable, Category = "My Functions")
	void MyFunction();
	
	/* Unreal Relfection system
	
	* BlueprintReadOnly -> Only read the value
	* BlueprintReadWrite
	* EditInstanceOnly -> this particular variable is editable only on an instance
	* EditAnywhere
	* EditDefaultsOnly
	* VisibleInstanceOnly
	* VisibleAnywhere -> Only visible into the editor but not editable
	* VisibleDefaultsOnly
	
	*/


};
