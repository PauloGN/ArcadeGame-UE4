// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ColliderMovementComponent.h"

// Sets default values
ACollider::ACollider(): CameraInput(FVector2D(0.0f))
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//SphereComponent->SetupAttachment(GetRootComponent());
	SetRootComponent(SphereComponent);

	SphereComponent->InitSphereRadius(40.f);
	//Collision Presets
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	//Not good Idea just for learning proposes
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshComponentAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;

	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	MyMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("MyMovementComponent"));
	//
	MyMovementComponent->UpdatedComponent = RootComponent;
	//Possess pawn
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	//NewRotation.Pitch += CameraInput.Y;
	SetActorRotation(NewRotation);

	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	//NewSpringArmRotation.Pitch += CameraInput.Y;
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch+= CameraInput.Y, -80.f, -15.f );

	SpringArm->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ThisClass::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);

}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return MyMovementComponent;
}

void ACollider::MoveForward(float value)
{
	FVector Foraward = GetActorForwardVector();
	//AddMovementInput(value * Foraward);
	//GEngine->AddOnScreenDebugMessage(2, 15, FColor::Cyan, TEXT("TESTEEEEEEEEEE"));

	if (MyMovementComponent)
	{
		MyMovementComponent->AddInputVector(Foraward * value);
	}
}

void ACollider::MoveRight(float value)
{
	FVector Right = GetActorRightVector();
	//AddMovementInput(value * Right);
	//GEngine->AddOnScreenDebugMessage(1, 15, FColor::Red, TEXT("Right"));

	if (MyMovementComponent)
	{
		MyMovementComponent->AddInputVector(Right * value);
	}
}

void ACollider::PitchCamera(float Axisvalue)
{
	CameraInput.Y = Axisvalue;
}

void ACollider::YawCamera(float Axisvalue)
{
	CameraInput.X = Axisvalue;
}

