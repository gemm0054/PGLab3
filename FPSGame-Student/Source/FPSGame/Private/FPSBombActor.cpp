// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBombActor.h"
#include "kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/DamageType.h"
#include "BombDamageType.h"
// Sets default values
//LAB 3: Complete the Constructor
AFPSBombActor::AFPSBombActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CREATE the BombBox Component
	
	//SET the BombBox's profile name to BlockAllDynamic

	//SUBSCRIBE to the BombBox's Hit event using the function OnHit()
																	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	//CALL SetWalkableSlopeOverride() on BombBox passing in WalkableSlope_Unwalkable, 0.f)
	
	//SET CanCharacterStepUpOn on BombBox to ECB_No
	

	//SET BombBox as the RootComponent
	

	//CREATE the BombMesh
	
	//ATTACH it to the RootComponent
	
	//CALL SetCollisionEnabled() on BombMesh passing in ECollisionEnabled::NoCollision
	

	//CREATE the RadialForceComp
	
	//ATTACH the RadialForceComp to the RootComponent
	
	//SET the RadialForceComp's Radius to 250 or a value of your choice
	
	//SET the RadialForceComp's bImpulseVelChange property to true
	
	//SET the RadialForceComp's bAutoActivate property to false. This will prevent the RadialForceComp from ticking and we will only use FireImpulse() instead
																				 // Prevent component from ticking, and only use FireImpulse() instead
	//SET the RadialForceComp's bIgnoreOwningActor property to true (ignoring self)
	


	//CALL SetSimulatePhysics() on the BombBox to true
	
	//CALL SetCollisionEnabled() on BombBox and enable it to respond to Queries and Physics Collision
	
	//CALL SetCollisionResponseToChannel() on the BombBox, Make it Ignore the Pawn


	//SET ExplodeDelay to 2.0f
	


}

//LAB 3: Complete Explode()
void AFPSBombActor::Explode()
{
	//CALL function to Play The Particle Effect
	

	//BLAST away nearby physics actors by Calling FireImpulse() on the Radial Force Component
	

	//APPLY RADIAL DAMAGE
	//UGameplayStatics::ApplyRadialDamage(this, 100, GetActorLocation(), 250, UDamageType::StaticClass(), TArray<AActor*>(), this);
	
	//CREATE a Timer to Destroy this Actor After 1 second
	
	
}

//LAB 3: Complete DestroyBomb()
void AFPSBombActor::DestroyBomb()
{
	//DESTROY this Actor

}
//LAB 3: Complete PlayParticleEffect()
void AFPSBombActor::PlayParticleEffect()
{
	//SPAWN the ExplosionTemplate using UGameplayStatics::SpawnEmitterAtLocation(....)
	
}

// Called when the game starts or when spawned
void AFPSBombActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSBombActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//LAB 3: Complete OnHit()
void AFPSBombActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{


	//CREATE a Timer to "Explode" this Actor After ExplodeDelay seconds
	
}

//LAB 3: Complete Hold()
void AFPSBombActor::Hold(USkeletalMeshComponent* HoldingComponent)
{
	//IF HoldingComponent
	if (HoldingComponent)
	{
		//DISABLE Physics
		
		//ATTACH the BombBox to the HoldingComponent at the HoldingComponents "Muzzle" Socket

	}
	//ENDIF
}

//LAB 3: Complete Throw()
void AFPSBombActor::Throw(FVector direction)
{
	//ENABLE physics on BombBox
	
	//CALL SetNotifyRigidBodyCollision() on BombBox and pass in true

	//CALL SetCollisionEnabled() on BombBox passing in ??::QueryAndPhysics

	//CALL SetCollisionResponseToChannel() on BombBox and make it Block with Pawn

	//DETACH BombBox

	//CALL AddForce(...) to BombBox to throw it, using the direction passed in

}

