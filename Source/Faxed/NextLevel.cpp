// Fill out your copyright notice in the Description page of Project Settings.


#include "NextLevel.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

// Sets default values
ANextLevel::ANextLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANextLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANextLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

