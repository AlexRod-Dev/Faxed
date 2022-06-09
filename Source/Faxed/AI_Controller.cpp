// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI_Character.h"
#include "Components/PrimitiveComponent.h"
#include "Waypoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FaxedCharacter.h"
#include "Perception/AISenseConfig_Sight.h"

AAI_Controller::AAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;
	
	

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);


}

void AAI_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Problem Occured"));
	}

	

}

void AAI_Controller::OnPossess(APawn* IPawn)
{
	Super::OnPossess(IPawn);
}

void AAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AAI_Character* aiCharacter = Cast<AAI_Character>(GetPawn());
	

	if (DistanceToPlayer > AISightRadius)
	{
		bIsPlayerDetected = false;
	}

	//Move to waypoint
	if (aiCharacter->NextWaypoint != nullptr && bIsPlayerDetected == false) {
		
		MoveToActor(aiCharacter->NextWaypoint, 5.0f);
		
	}
	//Move to Player
	else if (bIsPlayerDetected == true) 
	{
		aiCharacter->ViewCone->SetMaterial(0, aiCharacter->AlertMaterial);
		
		AFaxedCharacter* Player = Cast<AFaxedCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		Player->ActivateRagdoll();
		
		MoveToActor(Player, 5.0f);

		Player->bIsCaught = true;

		
		
	
	}
}

FRotator AAI_Controller::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);

}

void AAI_Controller::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

		bIsPlayerDetected = true;
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
	}
}