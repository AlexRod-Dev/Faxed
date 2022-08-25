// Copyright Epic Games, Inc. All Rights Reserved.

#include "FaxedCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AFaxedCharacter

AFaxedCharacter::AFaxedCharacter()
{

	bIsCrouching = false;
	walkSpeed = 300.0f;
	crouchSpeed = 200.0f;
	bIsCaught = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	PawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFaxedCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFaxedCharacter::StartCrouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFaxedCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFaxedCharacter::StopCrouch);

	

	PlayerInputComponent->BindAxis("MoveForward", this, &AFaxedCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFaxedCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AFaxedCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AFaxedCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFaxedCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFaxedCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFaxedCharacter::OnResetVR);
}




void AFaxedCharacter::OnResetVR()
{
	// If Faxed is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Faxed.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFaxedCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AFaxedCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AFaxedCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFaxedCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFaxedCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

	}
}

void AFaxedCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		
	}
}

void AFaxedCharacter::StartCrouch() {
	GetCapsuleComponent()->SetCapsuleHalfHeight(48.0f);
	GetCharacterMovement()->MaxWalkSpeed = crouchSpeed;
	AFaxedCharacter::Crouch();

	bIsCrouching = true;

}

void AFaxedCharacter::StopCrouch() {

	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	AFaxedCharacter::UnCrouch();

	bIsCrouching = false;
}


void AFaxedCharacter::ToggleCrouch() {
	
	if (bIsCrouching)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		AFaxedCharacter::UnCrouch();

		bIsCrouching = false;

	}
	else 
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(48.0f);
		GetCharacterMovement()->MaxWalkSpeed = crouchSpeed;
		AFaxedCharacter::Crouch();

		bIsCrouching = true;

	}
}



bool AFaxedCharacter::IsAnimationBlended() {
	return false;
}


void AFaxedCharacter::ReportNoise(USoundBase* SoundToPlay, float Volume)
{
	//if we have a sound to play
	if(SoundToPlay)
	{
		//Play Sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),SoundToPlay, GetActorLocation(), Volume);

		//Report Sound to AI
		MakeNoise(Volume, this, GetActorLocation());
			
	}
}

void AFaxedCharacter::RestartLevel()
{
	UWorld* ThisWorld = GetWorld();

	FString CurrentLevel = ThisWorld->GetName();
	
	
	UGameplayStatics::OpenLevel(GetWorld(), *CurrentLevel);
	
}

void AFaxedCharacter::ActivateRagdoll()
{
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	SetActorEnableCollision(true);
	
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;
}



