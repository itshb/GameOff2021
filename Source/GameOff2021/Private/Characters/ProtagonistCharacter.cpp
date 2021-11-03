// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ProtagonistCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AttributesComponent.h"
#include "Actors/Weapons/WeaponBase.h"

AProtagonistCharacter::AProtagonistCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	GetCapsuleComponent()->InitCapsuleSize(30.0f, 88.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 800.0f);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 6.5f;
	CameraBoom->CameraLagMaxDistance = 100.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));

	MaxWalkSpeed = 500.0f;
	MaxWalkSpeedSneaking = 200.0f;
	MaxWalkSpeedCrouched = 200.0f;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxWalkSpeedCrouched;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	GetMesh()->CanCharacterStepUpOn = ECB_No;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
	MaxHealth = 100;
}

void AProtagonistCharacter::BeginPlay() {
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxWalkSpeedCrouched;

	Attributes->Initialize(MaxHealth);
}

void AProtagonistCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(bPressedInteract) bPressedInteract = false;
}

void AProtagonistCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProtagonistCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProtagonistCharacter::MoveRight);
	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &AProtagonistCharacter::Evade);
	PlayerInputComponent->BindAction("Sneak", IE_Pressed, this, &AProtagonistCharacter::Sneak);
	PlayerInputComponent->BindAction("Sneak", IE_Released, this, &AProtagonistCharacter::StopSneaking);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AProtagonistCharacter::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AProtagonistCharacter::UnCrouch);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AProtagonistCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Released, this, &AProtagonistCharacter::StopPrimaryAttacking);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AProtagonistCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Released, this, &AProtagonistCharacter::StopSecondaryAttacking);
	PlayerInputComponent->BindAction("TertiaryAttack", IE_Pressed, this, &AProtagonistCharacter::TertiaryAttack);
	PlayerInputComponent->BindAction("TertiaryAttack", IE_Released, this, &AProtagonistCharacter::StopTertiaryAttacking);
	PlayerInputComponent->BindAction("ReloadWeapon", IE_Pressed, this, &AProtagonistCharacter::ReloadEquippedWeapon);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AProtagonistCharacter::Interact);
}

void AProtagonistCharacter::MoveForward(const float Value) {
	if(Controller && Value != 0.0f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AProtagonistCharacter::MoveRight(const float Value) {
	if(Controller && Value != 0.0f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AProtagonistCharacter::Evade() {
	bEvading = true;

	if(EquippedWeapon) {
		if(EquippedWeapon->IsReloading()) EquippedWeapon->StopReloading();
	}
}

void AProtagonistCharacter::OnEvadeFinished() {
	bEvading = false;
}

void AProtagonistCharacter::Sneak() {
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedSneaking;
	bSneaking = true;
}

void AProtagonistCharacter::StopSneaking() {
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	bSneaking = false;
}

void AProtagonistCharacter::Crouch() {
	Super::Crouch();
	bCrouching = true;
}

void AProtagonistCharacter::UnCrouch() {
	Super::UnCrouch();
	bCrouching = false;
}

void AProtagonistCharacter::PrimaryAttack() {
	if(EquippedWeapon) {
		EquippedWeapon->Fire();
		bPrimaryAttacking = true;
	}
}

void AProtagonistCharacter::StopPrimaryAttacking() {
	if(!bPrimaryAttacking) return;
	EquippedWeapon->StopFiring();
	bPrimaryAttacking = false;
}

void AProtagonistCharacter::SecondaryAttack() {
	bSecondaryAttacking = true;
	NotifyDamage(this, this, 10);
}

void AProtagonistCharacter::StopSecondaryAttacking() {
	bSecondaryAttacking = false;
}

void AProtagonistCharacter::TertiaryAttack() {
	bTertiaryAttacking = true;
}

void AProtagonistCharacter::StopTertiaryAttacking() {
	bTertiaryAttacking = false;
}

void AProtagonistCharacter::ReloadEquippedWeapon() {
	if(!EquippedWeapon) return;
	EquippedWeapon->Reload();
}

void AProtagonistCharacter::Interact() {
	bPressedInteract = true;
	EquipWeapon(Cast<AWeaponBase>(WeaponToEquip));
}

void AProtagonistCharacter::EquipWeapon(class AWeaponBase* InWeapon) {
	if(EquippedWeapon != nullptr) EquippedWeapon->Destroy();

	AActor* Spawned = GetWorld()->SpawnActor<AWeaponBase>(WeaponToEquip, GetMesh()->GetSocketLocation(WeaponSocketName), GetActorRotation());

	EquippedWeapon = Cast<AWeaponBase>(Spawned);

	Spawned->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, WeaponSocketName);
	Spawned->SetActorRelativeRotation(FRotator(0.0f, 85.0f, 0.0f));

	OnWeaponChanged.Broadcast(EquippedWeapon);
}

void AProtagonistCharacter::TakeDamage(class AActor* InstigatorA, const int32 Amount) {
	Attributes->Damage(Amount);
}

bool AProtagonistCharacter::CanSneak() const {
	return (!bSneaking && !bCrouching && !bEvading);
}

bool AProtagonistCharacter::CanCrouch() const {
	return (!bCrouching && !bEvading);
}

bool AProtagonistCharacter::CanEvade() const {
	return (!bEvading);
}

bool AProtagonistCharacter::CanPrimaryAttack() const {
	return (!bPrimaryAttacking && !bSecondaryAttacking && !bTertiaryAttacking && !bEvading);
}

bool AProtagonistCharacter::CanSecondaryAttack() const {
	return (!bPrimaryAttacking && !bSecondaryAttacking && !bTertiaryAttacking && !bEvading);
}

bool AProtagonistCharacter::CanTertiaryAttack() const {
	return (!bPrimaryAttacking && !bSecondaryAttacking && !bTertiaryAttacking && !bEvading);
}
