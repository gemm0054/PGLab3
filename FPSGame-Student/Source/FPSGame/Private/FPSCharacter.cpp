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
void AFPSCharacter::TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Damage Received - " + FString::FromInt(Damage));
	//SET the damage type to a UBombDamageType. Use Cast<UBombDamageType>(DamageType)
	
	//IF damageType

	{
		//Get The Capsule Component and disable physics

		//Disable Input

	}
	//ENDIF

}

AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation (FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation (FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation ( FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");

//LAB 3:
#pragma region LAB3
	//SUBSCRIBE to OnTakeAnyDamage Event. Use AddDynamic() and pass in this and ??
	OnTakeAnyDamage.AddDynamic(this, &AFPSCharacter::TakeAnyDamage);

	//SET HeldBomb to null
	HeldBomb = nullptr;
#pragma endregion	
	//TODO NETWORK: Replicate this Pawn


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


//LAB 3: Complete Fire()
void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		//GET the Gun's Muzzle Location and Rotation. Use GetSocketLocation() and GetSocketRotation() on GunMeshComponent


		//SET Spawn Collision Handling Override
	

		// SPAWN the projectile at the muzzle

	}

	// TRY and play the sound if specified


	// TRY and play a firing animation if specified

}

//LAB 3: Complete PlaySound()
void AFPSCharacter::PlaySound()
{
	// TRY and play the sound if specified
	//IF FireSound
	if (FireSound)
	{
		//PLAY the sound at the character's location using UGameplayStatics::PlaySoundAtLocation(....)

	}
	//ENDIF

}

//LAB 3: Complete PlayAnimation()
void AFPSCharacter::PlayAnimation()
{
	//If we have a FireAnimation
	
	{
		//Get the animation object for the arms mesh
		//ASSIGN AnimInstance to the Mesh1PComponent's AnimInstance. Create a local variable called AnimInstance of type UAnimInstance* 
		
		//IF AnimInstance
		
		{
			//Play the FireAnimation on the "Arms" slot with a blend in time of 0.0f. Use PlaySlotAnimationAsDynamicMontage(....)
			
		}
		//ENDIF
	}
	//END IF
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
//LAB 3: Complete PickupBomb()
void AFPSCharacter::PickupBomb()
{
	//RAYCAST to check if a Actor is hit by a raycast
	
	//IF a Actor has been detected/hit

	{
		//CHECK if it is a AFPSBombActor by doing a cast
		
		//IF it is a AFPSBombActor
		
		{
			//SET HeldBomb to the hit bomb
			
			//CALL Hold on the bomb passing in GunMeshComponent, we will attach it to the Gun
			
		}
		//ENDIF
		//
	}
	//ENDIF
}

//LAB 3: Complete ThrowBomb()
void AFPSCharacter::ThrowBomb()
{
	//IF we have a HeldBomb
	
	{
		//THROW the Held Bomb passing in the Camera's Forward Vector
		
	}
	//ENDIF
}


//LAB 3: Complete RayCastGetActor()
AActor* AFPSCharacter::RayCastGetActor()
{
	if (Controller && Controller->IsLocalPlayerController()) // we check the controller because we don't want bots to grab the use object and we need a controller for the GetPlayerViewpoint function
	{
		//DECLARE a StartTrace, Direction and EndTrace FVector


		//CALL SetupRay passing in StartTrace, Direction


		//Create a FCollisionQueryParams object called TraceParams and set the TraceParams.AddIgnoredActor(this), bTraceComplex to true and bReturnPhysicalMaterial to true;
		

		//Create a FHitResult object called Hit
		FHitResult Hit;
		//Get the World

		
		//CALL LineTraceSingleByChannel on the World passing in Hit, StartTrace, EndTrace, ECC_PhysicsBody, TraceParams
		
		//CALL DrawDebugLine on the World passing in StartTrace, EndTrace, FColor::Green, false, 1, 0, 1.f
		
		return Hit.GetActor();
	}
	return nullptr;
}

//LAB 3: Complete SetupRay()
void AFPSCharacter::SetupRay(FVector& StartTrace, FVector& Direction, FVector& EndTrace)
{
	//DECLARE a CamLoc and CamRot FVector and FRotator


	//Call GetPlayerViewPoint on the Controller passing in CamLoc, CamRot
	
	//CamLoc = GetActorLocation();

	//SET StartTrace to the Camera Location
	
	//SET Direction to the Camera Rotation Vector
	
	//SET EndTrace to StartTrace + Direction * 300
	
}
#pragma endregion
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

