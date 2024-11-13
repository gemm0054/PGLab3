#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// Set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	// TODO NETWORK: Set Replication on this actor

}

// LAB 3: Complete OnHit()
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// TODO NETWORK: We only want to do this on the server
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
	    // Add impulse to the Other Component to push it upon collision
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);

		// CALL SetRandomColor, pass in OtherComp and a random color
		SetRandomColor(OtherComp, FLinearColor::MakeRandomColor());

		// CALL SetScale, pass in OtherActor and OtherComp
		SetScale(OtherActor, OtherComp);

		// DESTROY this Actor (the projectile)
		Destroy();
	}
}

// LAB 3: Complete SetScale()
void AFPSProjectile::SetScale(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	// GET the Hit Actor's Scale using GetComponentScale()
	FVector Scale = OtherComp->GetComponentScale();

	// SCALE it by 0.8 (reduce by 20%)
	Scale *= 0.8f;

	// IF the Scale is less than 0.5 in any dimension
	if (Scale.GetMin() < 0.5f)
	{
		// DESTROY the Hit Actor if the scale is too small
		OtherActor->Destroy();
	}
	else
	{
		// SCALE the Hit Actor to the new Scale value
		OtherComp->SetWorldScale3D(Scale);
	}
}

// LAB 3: Complete SetRandomColor()
void AFPSProjectile::SetRandomColor(UPrimitiveComponent* OtherComp, FLinearColor color)
{
	// CREATE a dynamic material instance for the mesh surface
	UMaterialInstanceDynamic* MatInst = OtherComp->CreateAndSetMaterialInstanceDynamic(0);

	// IF the MatInst is successful (not null)
	if (MatInst)
	{
		// SET the MatInst to a RANDOM color using SetVectorParameterValue
		MatInst->SetVectorParameterValue(FName("Color"), color);
	}
}
