// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

#include "Engine.h"


UPlayerAnimInstance::UPlayerAnimInstance() {
	
	bIsCrouching = false;
	bIsMoving = false;
	speed = 0.f;
}


void UPlayerAnimInstance::NativeInitializeAnimation() {

	Super::NativeInitializeAnimation();

	//cache the pawn
	Owner = TryGetPawnOwner();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX) {

	//double check our pointers make sure nothing is empty
	if (!Owner) {
		return;
	}

	if (Owner->IsA(AFaxedCharacter::StaticClass())) {
		AFaxedCharacter* PlayerCharacter = Cast<AFaxedCharacter>(Owner);
		//again check pointers
		if (PlayerCharacter) {
			bIsAnimationBlended = PlayerCharacter->IsAnimationBlended();
			speed = PlayerCharacter->GetVelocity().Size();
			bIsCrouching = PlayerCharacter->GetMovementComponent()->IsCrouching();
			UE_LOG(LogTemp, Warning, TEXT("crouching is %s"), (bIsCrouching ? TEXT("true") : TEXT("false")));
			bIsMoving = PlayerCharacter->GetVelocity().Size() > 0 ? true : false;
		}
	}
}