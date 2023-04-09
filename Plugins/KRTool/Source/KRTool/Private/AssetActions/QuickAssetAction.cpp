// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"

void UQuickAssetAction::TestFunc()
{
	DebugHeader::ShowMessage(EAppMsgType::Ok, TEXT("Hello KRTool."));
}

void UQuickAssetAction::DuplicateAssets(int32 Count)
{
	if (Count <= 0)
	{
		DebugHeader::ShowMessage(EAppMsgType::Ok, TEXT("Please enter a valid count."));
		return;
	}

	TArray<FAssetData> SelectedAssetDatas = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 SuccessCount = 0;

	for (const FAssetData& AssetData : SelectedAssetDatas)
	{
		for (int32 i = 0; i < Count; i++)
		{
			const FString SourceAssetPath = AssetData.ObjectPath.ToString();
			const FString TargetAssetName = AssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i + 1);
			const FString TargetAssetPath = FPaths::Combine(AssetData.PackagePath.ToString(), TargetAssetName);

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, TargetAssetPath))
			{
				++SuccessCount;
				UEditorAssetLibrary::SaveAsset(TargetAssetPath, false);
			}
		}
	}

	if (SuccessCount > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully duplicated ") + FString::FromInt(SuccessCount) + " files.");
	}
}

void UQuickAssetAction::AddPrefixes()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (!SelectedObject) continue;

		FString* PrefixFound = PrefixMap.Find(SelectedObject->GetClass());

		if (!PrefixFound || PrefixFound->IsEmpty())
		{
			DebugHeader::ShowMessage(EAppMsgType::Ok, TEXT("Failed to find prefix for class ") +
												SelectedObject->GetClass()->GetName());
			return;
		}

		FString OldName = SelectedObject->GetName();

		// Ԥ��������
		// �Ƴ� MaterialInstance �� M_ �� _inst
		if (SelectedObject->IsA<UMaterialInstanceConstant>())
		{
			OldName.RemoveFromStart(TEXT("M_"));
			OldName.RemoveFromEnd(TEXT("_Inst"));
		}
		// ��Щ����ϰ�߲�ͬ����������
		else if (SelectedObject->IsA<UAnimSequence>() && OldName.StartsWith(TEXT("Anim_")))
		{
			OldName.RemoveFromStart(TEXT("Anim_"));
		}
		// �����С��ǰ���и� "_"
		else if (OldName.StartsWith(TEXT("_")))
		{
			OldName.RemoveFromStart(TEXT("_"));
		}

		if (OldName.StartsWith(*PrefixFound + TEXT("_")))
		{
			continue;
		}
		
		const FString NewName = *PrefixFound + TEXT("_") + OldName;
		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
		++Counter;
	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully renamed ") + FString::FromInt(Counter) + TEXT(" assets."));
	}
}

void UQuickAssetAction::RemoveUnusedAssets()
{
	FixUpRedirectors();

	TArray<FAssetData> SelectedAssetDatas = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetsDatas;

	for (const FAssetData& AssetData : SelectedAssetDatas)
	{
		TArray<FString> AssetReferences =
			UEditorAssetLibrary::FindPackageReferencersForAsset(AssetData.ObjectPath.ToString());

		if (AssetReferences.Num() == 0)
		{
			UnusedAssetsDatas.Add(AssetData);
		}
	}

	if (UnusedAssetsDatas.Num() == 0)
	{
		DebugHeader::ShowMessage(EAppMsgType::Ok, TEXT("No unused asset found among selected assets."));
		return;
	}

	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsDatas, true);
	if (NumOfAssetsDeleted == 0) return;

	DebugHeader::ShowNotifyInfo(TEXT("Successfully deleted ") + FString::FromInt(NumOfAssetsDeleted) + " unused files.");
}

void UQuickAssetAction::FixUpRedirectors()
{
	TArray<UObjectRedirector*> Redirectors;

	// ��ȡ�� FAssetRegistryModule
	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	// ����������
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace("/Game");
	Filter.ClassNames.Emplace("ObjectRedirector");

	// ʹ�ù������õ�������Ҫ�ض�����ʲ�
	TArray<FAssetData> OutRedirectorAssetDatas;
	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectorAssetDatas);

	// д�뵽 Redirectors ����
	for (const FAssetData RedirectorData : OutRedirectorAssetDatas)
	{
		if (UObjectRedirector* Redirector = Cast<UObjectRedirector>(RedirectorData.GetAsset()))
		{
			Redirectors.Add(Redirector);
		}
	}

	// ��ȡ�� FAssetToolsModule
	FAssetToolsModule& AssetToolsModule =
		FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	// �����ض���
	AssetToolsModule.Get().FixupReferencers(Redirectors);
}
