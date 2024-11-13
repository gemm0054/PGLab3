// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSBombActor.h"
#include "DrawDebugHelpers.h"
#include "BombDamageType.h"
#include "Kismet/KismetMathLibrary.h"

//LAB 3: Complete TakeAnyDamage()
void AFPSCharacter::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Damage Received - " + FString::FromInt(Damage));
	
	// Cast the damage type to UBombDamageType
	const UBombDamageType* BombDamageType = Cast<UBombDamageType>(DamageType);
	if (BombDamageType)
	{
		// Disable physics on the capsule component
		GetCapsuleComponent()->SetSimulatePhysics(false);

		// Disable player input
		DisableInput(Cast<APlayerController>(GetController()));
	}
}


AFPSCharacter::AFPSCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); 
	CameraComponent->bUsePawnControlRotation = true;

	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");

	OnTakeAnyDamage.AddDynamic(this, &AFPSCharacter::TakeAnyDamage);
	HeldBomb = nullptr;

	bReplicates = true;
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
#pragma region ICA1
	PlayerInputComponent->BindAction("SpawnBomb", IE_Pressed, this, &AFPSCharacter::PickupBomb);
	PlayerInputComponent->BindAction("SpawnBomb", IE_Released, this, &AFPSCharacter::ThrowBomb);
#pragma endregion
}


void AFPSCharacter::Fire()
{
	if (ProjectileClass)
	{
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	}

	PlaySound();
	PlayAnimation();
}

void AFPSCharacter::PlaySound()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}


void AFPSCharacter::PlayAnimation()
{
	if (FireAnimation)
	{
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}


////////////////////////////////////// ICA1 ///////////////////////////////////////////////////////////////////////////
#pragma region LAB3
void AFPSCharacter::PickupBomb()
{
	// Raycast to detect if an actor is hit
	AActor* HitActor = RayCastGetActor();
	if (HitActor)
	{
		// Cast the hit actor to AFPSBombActor
		AFPSBombActor* HitBomb = Cast<AFPSBombActor>(HitActor);
		if (HitBomb)
		{
			// Debugging: Check if the bomb is detected
			UE_LOG(LogTemp, Warning, TEXT("Bomb detected!"));

			// Set HeldBomb to the detected bomb and hold it
			HeldBomb = HitBomb;
			HeldBomb->Hold(GunMeshComponent);
		}
	}
}



//LAB 3: Complete ThrowBomb()
void AFPSCharacter::ThrowBomb()
{
	if (HeldBomb)
	{
		// Throw the bomb in the camera's forward direction
		HeldBomb->Throw(CameraComponent->GetForwardVector());
		HeldBomb = nullptr; // Reset HeldBomb as it is no longer held
	}
}



AActor* AFPSCharacter::RayCastGetActor()
{
	if (Controller && Controller->IsLocalPlayerController())
	{
		FVector StartTrace, Direction, EndTrace;
		SetupRay(StartTrace, Direction, EndTrace);

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);  // Avoid hitting the player itself
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_PhysicsBody, TraceParams);

		if (bHit)
		{
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 1, 0, 1.f);

			// If we hit a component, try to get the actor
			if (Hit.GetComponent())
			{
				AActor* HitActor = Hit.GetComponent()->GetOwner();
				if (HitActor && HitActor->IsA(AFPSBombActor::StaticClass()))
				{
					UE_LOG(LogTemp, Warning, TEXT("Raycast hit the bomb actor: %s"), *HitActor->GetName());
					return HitActor;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Raycast hit component but not a bomb actor"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Raycast hit but no valid component or actor found"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Raycast did not hit anything"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is not a local player controller"));
	}

	return nullptr;
}

//LAB 3: Complete SetupRay()
void AFPSCharacter::SetupRay(FVector& StartTrace, FVector& Direction, FVector& EndTrace)
{
	// Get the camera location and rotation
	FVector CamLoc;
	FRotator CamRot;
	Controller->GetPlayerViewPoint(CamLoc, CamRot);

	// Set up start trace, direction, and end trace for raycast
	StartTrace = CamLoc;
	Direction = CamRot.Vector();
	EndTrace = StartTrace + Direction * 300.0f; // Adjust range as needed
}

#pragma endregion
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

