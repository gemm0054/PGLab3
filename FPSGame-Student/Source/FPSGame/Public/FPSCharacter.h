// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class AFPSBombActor;


UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

#pragma region ICA1
	//TODO NETWORK: REPLICATE this Variable	
	class AFPSBombActor* HeldBomb;

	UFUNCTION()
		void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
#pragma endregion
public:
	AFPSCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

#pragma region ICA1
	UPROPERTY(EditDefaultsOnly, Category = "Bombs")
		TSubclassOf<AFPSBombActor> BombClass;
#pragma endregion
protected:
	
	/** Fires a projectile. */
	//TODO NETWORK: Fire needs to be done on the Server
	void Fire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

#pragma region ICA1
	//TODO NETWORK: PicupBomb needs to be done on the server
	void PickupBomb();
	//TODO NETWORK: ThrowBomb Needs to be done on the server
	void ThrowBomb();
	
	//TODO NETWORK: ADD a NetMulticast Function to Play Sound on the Clients
	void PlaySound();

	//TODO NETWORK: ADD a NetMulticast Function to Play The ANimation on the Clients
	void PlayAnimation();

	AActor* RayCastGetActor();

	void SetupRay(FVector& StartTrace, FVector& Direction, FVector& EndTrace);

#pragma endregion

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

};

