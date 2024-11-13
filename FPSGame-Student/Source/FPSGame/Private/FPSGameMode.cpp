// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	//TODO NETWORK: Uncomment This, We need to set all the StaticMesh Actors in the level to replicate
	for (TActorIterator<AStaticMeshActor> StaticmeshItr(GetWorld()); StaticmeshItr; ++StaticmeshItr)
	{
		StaticmeshItr->SetReplicates(true);
		FRepMovement mv;
		mv.bRepPhysics = true;

		StaticmeshItr->SetReplicatedMovement(mv);
		StaticmeshItr->GetRootComponent()->SetIsReplicated(true);


	}
}
