// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBombActor.h"
#include "kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/DamageType.h"
#include "BombDamageType.h"
#include "FPSCharacter.h"

// Sets default values
AFPSBombActor::AFPSBombActor()
{
 	PrimaryActorTick.bCanEverTick = true;

	// Create the BombBox Component
	BombBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BombBox"));
	BombBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BombBox->OnComponentHit.AddDynamic(this, &AFPSBombActor::OnHit);

	// Prevent characters from walking on it
	BombBox->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	BombBox->CanCharacterStepUpOn = ECB_No;

	// Set BombBox as the RootComponent
	RootComponent = BombBox;

	// Create BombMesh and attach it to the RootComponent
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	BombMesh->SetupAttachment(RootComponent);
	BombMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // No collision for visual mesh

	// Create the RadialForceComp and attach it
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->Radius = 250.0f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	// Enable physics on BombBox and set collision responses
	BombBox->SetSimulatePhysics(true);
	BombBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Enable raycasting and physics interaction
	BombBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Ignore player character for physics

	// Allow raycasts to interact with BombBox by setting the collision response
	BombBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // Block visibility channel (raycasting)

	// Set the explode delay (default 2 seconds)
	ExplodeDelay = 2.0f;
}

void AFPSBombActor::Explode()
{
	// Play the particle effect
	PlayParticleEffect();

	// Fire impulse to affect nearby physics actors
	RadialForceComp->FireImpulse();

	// Apply radial damage
	UGameplayStatics::ApplyRadialDamage(
		this,
		100.0f, // Damage amount
		GetActorLocation(),
		RadialForceComp->Radius,
		UDamageType::StaticClass(),
		TArray<AActor*>(), // Ignore actors list
		this,
		GetInstigatorController()
	);

	// Schedule bomb destruction after a delay
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AFPSBombActor::DestroyBomb);
}

void AFPSBombActor::DestroyBomb()
{
	Destroy();
}

void AFPSBombActor::PlayParticleEffect()
{
	if (ExplosionTemplate)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionTemplate,
			GetActorLocation(),
			GetActorRotation()
		);
	}
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

void AFPSBombActor::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	// Exclude player character from triggering the explosion
	if (OtherActor && !OtherActor->IsA(AFPSCharacter::StaticClass()))
	{
		// Schedule explosion after delay
		FTimerHandle ExplodeTimer;
		GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &AFPSBombActor::Explode, ExplodeDelay);
	}
}

void AFPSBombActor::Hold(USkeletalMeshComponent* AttachTo)
{
	if (AttachTo)
	{

		// Optionally disable physics and collisions to avoid unintended interactions
		BombBox->SetSimulatePhysics(false);
		// Attach the bomb to the given skeletal mesh component (e.g., the gun)
		AttachToComponent(AttachTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Muzzle");
	}
}

void AFPSBombActor::Throw(FVector Direction)
{
	BombBox->SetSimulatePhysics(true);
	BombBox->SetNotifyRigidBodyCollision(true);
	BombBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BombBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // Allow interaction with raycasts and physics

	// Detach from actor and apply force
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FVector imp = Direction * 1000.0f;
	BombBox->AddImpulse(imp, NAME_None, true);

	FTimerHandle ExplodeTimer;
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &AFPSBombActor::Explode, ExplodeDelay);
}
