// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Engine/Texture.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"
#include "LevelSequence.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimBlueprint.h"

#include "QuickAssetAction.generated.h"

/**
 *
 */
UCLASS()
class KRTOOL_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	/// <summary>
	/// ���Ժ�����ֱ�ӵ���
	/// </summary>
	UFUNCTION(CallInEditor)
		void TestFunc();

	/// <summary>
	/// ����ָ�������ĵ�ǰѡ����ʲ�
	/// </summary>
	/// <param name="Count">���Ƶ�����</param>
	UFUNCTION(CallInEditor)
		void DuplicateAssets(int32 Count);

	/// <summary>
	/// �Զ���ѡ�е��ʲ�����ǰ׺
	/// </summary>
	UFUNCTION(CallInEditor)
		void AddPrefixes();

	/// <summary>
	/// �Ƴ�δʹ�õ��ʲ�
	/// </summary>
	UFUNCTION(CallInEditor)
		void RemoveUnusedAssets();

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
		{UNiagaraSystem::StaticClass(), TEXT("FXS")},
		{UNiagaraEmitter::StaticClass(), TEXT("FXE")},
		{ULevelSequence::StaticClass(), TEXT("LS")},
		{UAnimSequence::StaticClass(), TEXT("AS")},
		{UAnimBlueprint::StaticClass(), TEXT("ABP")},
	};

	void FixUpRedirectors();
};
