// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GamePlayStatics.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"
#include "MainPlayerController.h"
#include "ArcadeSaveGame.h"
#include "ItemStorage.h"


// Sets default values
AMainCharacter::AMainCharacter(): BaseTurnRate(65.f), BaseLookUpRate(65.f),ActiveOverlappingItem(nullptr), EquippedWeapon(nullptr)
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

	/**
	* 
	*	Player Stats Default values
	* 
	*/
		MaxHealth = 100.f;
		Health = 75.f;
		MaxStamina = 150.f;
		Stamina = 120.f;
		Coins = 0;
		RunninSpeed = 650.f;
		SprintingSpeed = 900.f;
		MovementStatus = EMovementStatus::EMS_Normal;
		StaminaStatus = EStaminaStatus::ESS_Normal;
		bShiftKeydown = false;
		bActionPerformed = false;
		StaminaDrainRate = 25.f;
		MinSprintStamina = 50.f;

		//Animation
		CombatMontage = nullptr;
		bAttacking = false;
		bLeftMouseButtomDown = false;
		CombatTarget = nullptr;
		HitParticles = nullptr;
		HitSound = nullptr;

		//interpolation
		InterpSpeed = 15.f;
		bInterpToEnemy = false;

		/** */
		MainPlayerController = nullptr;
}

void AMainCharacter::ShowPickUpLocations()
{
	for (size_t i = 0; i < PickUpLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickUpLocations[i] + FVector(0.f, 0.f, 100.f), 15.f, 12, FColor::Blue, 5.f, 2.f);
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	MainPlayerController = Cast<AMainPlayerController>(GetController());
}

void AMainCharacter::SetInterptoEnemy(bool bIntep)
{
	bInterpToEnemy = bIntep;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Stamina status
	UpdateStaminaStatus(DeltaTime);

	//Interpolatting
	ActorFaceEnemy(DeltaTime);

	//Getting location to place the widget of the enemy
	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//Action Inputs 
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ThisClass::ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ThisClass::Sprinting_ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ThisClass::Running_ShiftKeyUp);
	//E key pressed
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ThisClass::ActionPerformed_E_Pressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ThisClass::ActionPerformed_E_UP);
	//Left Mouse Buttom key pressed
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ThisClass::AttackPerformed_LMB_Pressed);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ThisClass::AttackPerformed_LMB_UP);

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

	if (MovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	uint8 bCanMoveForward = (Controller != nullptr) && (input != 0.0f) && (!bAttacking);
	if (bCanMoveForward)
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

	if (MovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	uint8 bCanMoveRight = (Controller != nullptr) && (input != 0.0f) && (!bAttacking);

	if (bCanMoveRight)
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

void AMainCharacter::SetMovementStatus(EMovementStatus eStatus)
{

	MovementStatus = eStatus;

	switch (MovementStatus)
	{
	case EMovementStatus::EMS_Normal:

		GetCharacterMovement()->MaxWalkSpeed = RunninSpeed;

		break;
	case EMovementStatus::EMS_Sprinting:

		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;

		break;
	case EMovementStatus::EMS_MAX:
		break;
		GetCharacterMovement()->MaxWalkSpeed = RunninSpeed;
	}
}

void AMainCharacter::SetEquippedWeapon(AWeapon* weap)
{

	if (EquippedWeapon && weap->WeaponState == EWeaponState::EWS_Pickup)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = weap;
}

void AMainCharacter::Sprinting_ShiftKeyDown()
{
	bShiftKeydown = true;
}

void AMainCharacter::Running_ShiftKeyUp()
{
	bShiftKeydown = false;
}

void AMainCharacter::DecrementHealth(const float dmg)
{
	if (Health - dmg <= 0.f)
	{
		Health -= dmg;
		Die();
	}
	else
	{
		Health -= dmg;
	}

}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCouser)
{
	DecrementHealth(DamageAmount);
	return DamageAmount;
}

void AMainCharacter::IncrementCoin(const int32 value)
{
	if (Coins + value <= 1000)
	{
		Coins += value;
	}
	else
	{
		//Max coins collection
		Coins = 1000;
	}
}

void AMainCharacter::IncrementHealth(const float value)
{
	if (Health + value < MaxHealth)
	{
		Health += value;
	}
	else
	{
		Health = MaxHealth;
	}
}

void AMainCharacter::Die()
{

	if (MovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 0.6f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
		SetMovementStatus(EMovementStatus::EMS_Dead);
	}
}

void AMainCharacter::DeathEnd()
{
	GetMesh()->bNoSkeletonUpdate = true;
	GetMesh()->bPauseAnims = true;
}


void AMainCharacter::ActionPerformed_E_Pressed()
{

	if (MovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	bActionPerformed = true;

	if (ActiveOverlappingItem)
	{
		AWeapon* wep = Cast<AWeapon>(ActiveOverlappingItem);

		if (wep)
		{
			wep->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
}
void AMainCharacter::ActionPerformed_E_UP()
{
	bActionPerformed = false;
}

//Attack
void AMainCharacter::AttackPerformed_LMB_Pressed()
{
	if (MovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	bLeftMouseButtomDown = true;
	if (EquippedWeapon && (!bAttacking))
	{
		SetInterptoEnemy(true);
		bAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		const float SpeedRate = 2.f;
		const FName AttackFname = GetAttackAnimationName();

		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, SpeedRate);
			AnimInstance->Montage_JumpToSection(AttackFname, CombatMontage);
		}
	}
}

void AMainCharacter::AttackPerformed_LMB_UP()
{
	bLeftMouseButtomDown = false;
}

void AMainCharacter::Jump()
{
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
}

void AMainCharacter::UpdateCombatTarget()
{

	TArray<AActor*>OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	//Getting the closest enemy surounding the player
	if (ClosestEnemy)
	{
		FVector MyLocation = GetActorLocation();

		float MinDistance = (ClosestEnemy->GetActorLocation() - MyLocation).Size();

		for (auto OverlappingActor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(OverlappingActor);
			if (Enemy)
			{
				float ActorDistance = (Enemy->GetActorLocation() - MyLocation).Size();
				if (ActorDistance < MinDistance)
				{
					MinDistance = ActorDistance;
					ClosestEnemy = Enemy;
				}
			}
		}

		//Display the enemy helath bar
		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		//Set the new combat target to the closest enemy arround
		SetCombatTarget(ClosestEnemy);
	}
}

void AMainCharacter::Attackfinished()
{
	if (EquippedWeapon)
	{
		bAttacking = false;
		bInterpToEnemy = false;
		//if the mouse keep down attack again
		if (bLeftMouseButtomDown)
		{
			AttackPerformed_LMB_Pressed();
		}
	}
}


void AMainCharacter::SwitchLevel(const FName LevelName)
{

	UWorld* World = GetWorld();

	if (World)
	{
		//converting the fstring to string literal using the asterisc operator
		FName CurrentLevelName = *(World->GetMapName());
		if (CurrentLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
}

// SAVE AND LOAD

void AMainCharacter::SaveGame()
{
	UArcadeSaveGame* ArcadeSaveGameInstance =  Cast<UArcadeSaveGame>(UGameplayStatics::CreateSaveGameObject(UArcadeSaveGame::StaticClass()));

	if (ArcadeSaveGameInstance)
	{
		ArcadeSaveGameInstance->CharacterStats.Health = Health;
		ArcadeSaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
		ArcadeSaveGameInstance->CharacterStats.Stamina = Stamina;
		ArcadeSaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
		ArcadeSaveGameInstance->CharacterStats.Coins = Coins;

		if (EquippedWeapon)
		{
			ArcadeSaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->WeaponName;
		}

		ArcadeSaveGameInstance->CharacterStats.Location = GetActorLocation();
		ArcadeSaveGameInstance->CharacterStats.Rotation = GetActorRotation();

		UGameplayStatics::SaveGameToSlot(ArcadeSaveGameInstance, ArcadeSaveGameInstance->PlayerName, ArcadeSaveGameInstance->UserIndex);
	}
}

void AMainCharacter::LoadGame(bool bSetPosition)
{
	UArcadeSaveGame* ArcadeLoadGameInstance = Cast<UArcadeSaveGame>(UGameplayStatics::CreateSaveGameObject(UArcadeSaveGame::StaticClass()));

	if (ArcadeLoadGameInstance)
	{
		ArcadeLoadGameInstance = Cast<UArcadeSaveGame>(UGameplayStatics::LoadGameFromSlot(ArcadeLoadGameInstance->PlayerName, ArcadeLoadGameInstance->UserIndex));
		
		Health = ArcadeLoadGameInstance->CharacterStats.Health;
		MaxHealth = ArcadeLoadGameInstance->CharacterStats.MaxHealth;
		Stamina = ArcadeLoadGameInstance->CharacterStats.Stamina;
		MaxStamina = ArcadeLoadGameInstance->CharacterStats.MaxStamina;
		Coins = ArcadeLoadGameInstance->CharacterStats.Coins;

		if (bSetPosition)
		{
			SetActorLocation(ArcadeLoadGameInstance->CharacterStats.Location);
			SetActorRotation(ArcadeLoadGameInstance->CharacterStats.Rotation);
		}

		if (ItemStorage)
		{
			AItemStorage* Weapon =  GetWorld()->SpawnActor<AItemStorage>(ItemStorage);

			if (Weapon)
			{
				FString WeaponName = ArcadeLoadGameInstance->CharacterStats.WeaponName;

				if (Weapon->WeaponCollecionMAP.Contains(WeaponName))
				{
					AWeapon* WeaponToEquipe = GetWorld()->SpawnActor<AWeapon>(Weapon->WeaponCollecionMAP[WeaponName]);		
					WeaponToEquipe->Equip(this);
				}
			}
		}

		MovementStatus = EMovementStatus::EMS_Normal;
		GetMesh()->bNoSkeletonUpdate = false;
		GetMesh()->bPauseAnims = false;
	}
}

//Generate radon attack
FName AMainCharacter::GetAttackAnimationName()
{

	int32 Selection = FMath::RandRange(0, 1);
	FName AttackName = FName("null");


	switch (Selection)
	{
	case 0:
		AttackName = TEXT("Attack_1");
		break;
	case 1:
		AttackName = TEXT("Attack_2");
		break;

	default:
		;
	}

	return AttackName;

}

void AMainCharacter::UpdateStaminaStatus(float& DeltaTime)
{
	//No stamina/velocity/animation change if you are dead
	if (MovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	//No stamina/velocity/animation change if you are holding the leftshift key while not moving
	const FVector CurrentCharVelocity = GetCharacterMovement()->Velocity;
	if (CurrentCharVelocity.Size() == 0.f && bShiftKeydown)
	{
		SetMovementStatus(EMovementStatus::EMS_Normal);

		//Penalty to keep holding the left shift key.
		const float DeltaStamina = StaminaDrainRate * DeltaTime;
		Stamina -= DeltaStamina;

		if (Stamina - DeltaStamina <= 0.f)
		{
			Stamina = 0.f;
			SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
		}

		return;
	}

	//how much the stamina should change in this particular frame
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:

		if (bShiftKeydown)
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}

			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else//Shift is up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:

		if (bShiftKeydown)
		{

			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0.f;
				SetMovementStatus(EMovementStatus::EMS_Normal);

			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else//Shift is up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);

		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeydown)
		{
			Stamina = 0.f;
		}
		else//Shift is up
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else//Shift is up
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_MAX:
		break;
	default:
		break;
	}
}

FRotator AMainCharacter::LookAtRotationYaw(FVector Target)
{
	FRotator LootAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	return FRotator(0.f, LootAtRotation.Yaw, 0.f);
}

void AMainCharacter::ActorFaceEnemy(float DeltaTime)
{
	if (MovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = LookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}
}