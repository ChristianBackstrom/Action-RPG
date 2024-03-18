// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeHelpers.h"
#include "StandardAttributeSet.generated.h"

UCLASS()
class ARPG_AKC_API UStandardAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, HealthMax);
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, Damage)
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData HealthMax;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Damage;
	
	
	
};
