// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FaxedCharacter.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


 
 
UCLASS()
class FAXED_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	


protected:
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bIsAnimationBlended;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Animation")
		bool bIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bIsMoving;

public:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	
private:
	APawn* Owner;
};
