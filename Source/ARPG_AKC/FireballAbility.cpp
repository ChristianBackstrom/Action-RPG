// Fill out your copyright notice in the Description page of Project Settings.


#include "FireballAbility.h"

#include "Projectile.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetMathLibrary.h"

UFireballAbility::UFireballAbility()
{
	FGameplayTag FireballTag = FGameplayTag::RequestGameplayTag(FName("Action.Fireball"));
	AbilityTags.AddTag(FireballTag);
}

void UFireballAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	AActor* OwnerActor = GetAvatarActorFromActorInfo();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FVector WorldLocation, WorldDirection;
	
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FVector SpawnLocation = OwnerActor->GetActorForwardVector() * 100 + OwnerActor->GetActorLocation();

	FVector PlaneOrigin(0.0f, 0.0f, SpawnLocation.Z);


	FVector targetLocation = FMath::LinePlaneIntersection(
			WorldLocation,
			WorldLocation + WorldDirection,
			PlaneOrigin,
			FVector::UpVector);

	DrawDebugSphere(GetWorld(), targetLocation, 50, 10, FColor::White, false, 15.f);

	OwnerActor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), targetLocation));
	SpawnLocation = OwnerActor->GetActorForwardVector() * 100 + OwnerActor->GetActorLocation();
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, targetLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = OwnerActor;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(FireballActor, SpawnLocation, SpawnRotation, SpawnParameters);

	UAbilityTask_WaitGameplayEvent* waitForEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, 
	FGameplayTag::RequestGameplayTag("Event.Fireball"));

	// waitForEvent->EventReceived.AddDynamic(this, &UFireballAbility::FireballHit);
	EndThisAbility(false);
}

void UFireballAbility::EndThisAbility(bool bWasCanceled)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UFireballAbility::FireballHit(FGameplayEventData data)
{
	EndThisAbility(false);
}
