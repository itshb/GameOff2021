// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "ProtagonistCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChanged, class AWeapon*, NewEquippedWeapon);

UCLASS()
class GAMEOFF2021_API AProtagonistCharacter : public ACharacter, public IDamageableInterface {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UAttributesComponent* Attributes;

	bool bPressedInteract;

	bool bSneaking;

	bool bEvading;

	bool bCrouching;

	bool bPrimaryAttacking;

	bool bSecondaryAttacking;

	bool bTertiaryAttacking;

	class AWeapon* EquippedWeapon = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protagonist|Movement", meta = (ClampMin = "1.0"))
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protagonist|Movement", meta = (ClampMin = "1.0"))
	float MaxWalkSpeedSneaking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protagonist|Movement", meta = (ClampMin = "1.0"))
	float MaxWalkSpeedCrouched;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protagonist|Weapon")
	TSubclassOf<class AWeapon> WeaponToEquip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protagonist|Attributes", meta = (ClampMin = "1"))
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protagonist|Weapon")
	FName WeaponSocketName;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void MoveForward(const float Value);

	virtual void MoveRight(const float Value);

	virtual void Evade();

	void OnEvadeFinished();

	virtual void Sneak();

	virtual void StopSneaking();

	virtual void Crouch();

	virtual void UnCrouch();

	/* Sole purpose is pure destruction */
	virtual void PrimaryAttack();

	/* Sad face */
	virtual void StopPrimaryAttacking();

	/* E.g Toggle alternate firing mode */
	virtual void SecondaryAttack();

	virtual void StopSecondaryAttacking();

	/* E.g Throw grenades or plant mines */
	virtual void TertiaryAttack();

	virtual void StopTertiaryAttacking();

	virtual void ReloadEquippedWeapon();

	virtual void Interact();

	virtual void TakeDamage(class AActor* Instigator, const int32 Amount);

	void EquipWeapon(class AWeapon* InWeapon);

public:
	AProtagonistCharacter();

	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	class UCameraComponent* GetFollowCamera() const { return Camera; }

	class UAttributesComponent* GetAttributes() const { return Attributes; }

	UFUNCTION(BlueprintCallable)
	class AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	UPROPERTY(BlueprintAssignable)
	FOnEquippedWeaponChanged OnWeaponChanged;

	bool CanSneak() const;

	bool CanCrouch() const;

	bool CanEvade() const;

	bool CanPrimaryAttack() const;

	bool CanSecondaryAttack() const;

	bool CanTertiaryAttack() const;

	bool IsSneaking() const { return bSneaking; }

	bool IsCrouching() const { return bCrouching; }

	bool IsEvading() const { return bEvading; }

	bool IsPrimaryAttacking() const { return bPrimaryAttacking; }

	bool IsSecondaryAttacking() const { return bSecondaryAttacking; }

	bool IsTertiaryAttacking() const { return bTertiaryAttacking; }
};
