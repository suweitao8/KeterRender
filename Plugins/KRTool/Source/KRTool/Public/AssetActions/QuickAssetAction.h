// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Engine/Texture.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "QuickAssetAction.generated.h"


/**
 *
 */
UCLASS()
class KRTOOL_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
		void TestFunc();

	UFUNCTION(CallInEditor)
		void DuplicateAssets(int32 Count);

	UFUNCTION(CallInEditor)
		void AddPrefixes();

private:
	TMap<UClass*, FString> PrefixMap =
	{
		{UBlueprint::StaticClass(), TEXT("BP")},
		{UStaticMesh::StaticClass(), TEXT("SM")},
		{UMaterial::StaticClass(), TEXT("M")},
		{UMaterialInstanceConstant::StaticClass(), TEXT("MI")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MF")},
		{UParticleSystem::StaticClass(), TEXT("PS")},
		{USoundCue::StaticClass(), TEXT("SC")},
		{USoundWave::StaticClass(), TEXT("SW")},
		{UTexture::StaticClass(), TEXT("T")},
		{UTexture2D::StaticClass(), TEXT("T")},
		{UUserWidget::StaticClass(), TEXT("WBP")},
		{USkeletalMeshComponent::StaticClass(), TEXT("SK")},
		{UNiagaraSystem::StaticClass(), TEXT("NS")},
		{UNiagaraEmitter::StaticClass(), TEXT("NE")},
	};
};
