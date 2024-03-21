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
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, MaxMana);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxMana;
	

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
};
