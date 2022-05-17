// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Character.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAI_Character::AAI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,600.0f,0.0f);

	FVector ConeScale = FVector();
	
	ViewCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("View Cone"));
	ViewCone->SetupAttachment(RootComponent);
	ViewCone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ViewCone->SetWorldScale3D(ConeScale);
	ViewCone->SetRelativeScale3D(ConeScale);
	
	PatrolMaterial = CreateDefaultSubobject<UMaterial>(TEXT("PatrolMaterial"));
	AlertMaterial = CreateDefaultSubobject<UMaterial>(TEXT("AlertMaterial"));

}

// Called when the game starts or when spawned
void AAI_Character::BeginPlay()
{
	Super::BeginPlay();
	
	ViewCone->SetMaterial(0, PatrolMaterial);
}

// Called every frame
void AAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAI_Character::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetMesh()->GetSocketLocation("HeadSocket");
	OutRotation = GetMesh()->GetSocketRotation("HeadSocket");
}

