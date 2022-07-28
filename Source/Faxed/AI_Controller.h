// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FaxedCharacter.h"
#include "AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class FAXED_API AAI_Controller : public AAIController
{
	GENERATED_BODY()

public:

		AAI_Controller();

		

		virtual void BeginPlay() override;

		virtual void OnPossess(APawn* IPawn) override;

		virtual void Tick(float DeltaSeconds) override;

		virtual FRotator GetControlRotation() const override;

		UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 1000.0f;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.0f;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.0f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 75.0f;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;

		UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = AI)
		float DistanceToPlayer = 0.0f;

		bool bDoOnce = false;

		void RestartLevel();
	
		FTimerHandle TimerHandle;

};

