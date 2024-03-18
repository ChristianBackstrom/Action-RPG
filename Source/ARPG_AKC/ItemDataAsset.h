// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

struct FItemGenericInfo;
/**
 * 
 */
UCLASS()
class ARPG_AKC_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Item Info")
	TArray<FItemGenericInfo> ItemGenericInfos;
};
