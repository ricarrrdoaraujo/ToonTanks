// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectileBase.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	//Inherit the movement of CapsuleComp
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}

void APawnBase::RotateTurret(FVector LookAtTarget)
{
	// Uptadate TurretMesh rotation to face towards the LookAtTarget passed in from the child Class
	//TurretMesh->SetWorldRotation...

	FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z));

	//Rotate turret
	TurretMesh->SetWorldRotation(TurretRotation);

	//Alternative:
	/*FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = (LookAtTarget - StartLocation).Rotation();
	TurretRotation.Pitch = 0.f;
	TurretMesh->SetWorldRotation(TurretRotation);*/
}

void APawnBase::Fire()
{
	// Get ProjectileSpawnPoint Location && Rotation -> spawn projectile class at location towards rotation
	if (ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

void APawnBase::HandleDestruction()
{
	// Universal functionality
	// Play death effects Particle, sound and camera shake

	//then do unique child override
	// PawnTurret - inform GameMode Turret died -> then Destroy() self.

	// PawnTank - inform GameMode Player died -> then Hide() all components && stop movement Input.
}
