// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter(): BaseTurnRate(65.f), BaseLookUpRate(65.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;//Camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true;//Rotate arm based on controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;//camera attached to the end of the boom will let just the boom adjust the match of the controller orientation

	//Do not rotate the character along the Controller rotation
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	//Configure character movement to - makes character to move toward the direction of the inputs (W, S, A, D)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);//Rotation speed on yaw axis
	GetCharacterMovement()->JumpZVelocity = 650.f;//How hing the character jumps by default
	GetCharacterMovement()->AirControl = 0.5f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//Action Inputs 
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ThisClass::ACharacter::StopJumping);

	//KeyBoard inputs
	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	//Mouse Inputs based on inherited function from APawn
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//Keyboard arrows inputs - Rotation
	PlayerInputComponent->BindAxis("TurnRate", this, &ThisClass::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ThisClass::LookUpAtRate);

}

void AMainCharacter::MoveForward(float input)
{
	if ((Controller != nullptr) && (input != 0.0f))
	{
		//Find out witch way is forward from the controller perpective
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, input);
	}
}

void AMainCharacter::MoveRight(float input)
{
	if ((Controller != nullptr) && (input != 0.0f))
	{
		//Find out witch way is forward from the controller perpective
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, input);
	}
}

void AMainCharacter::TurnAtRate(float rate)
{
	UWorld* world = GetWorld();
	if (world)
	{
		const float rateInput = rate * BaseTurnRate * world->GetDeltaSeconds();
		AddControllerYawInput(rateInput);
	}
}

void AMainCharacter::LookUpAtRate(float rate)
{
	UWorld* world = GetWorld();
	if (world)
	{
		const float rateInput = rate * BaseTurnRate * world->GetDeltaSeconds();
		AddControllerPitchInput(rateInput);
	}
}

