// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"


void UQuickAssetAction::TestFunc()
{
	PrintGame(TEXT("Working Print Game"), FColor::Cyan);
	PrintConsole(TEXT("Working Print Console"));
}

void UQuickAssetAction::DuplicateAssets(int32 Count)
{
	if (Count <= 0)
	{
		ShowMessage(EAppMsgType::Ok, TEXT("Please enter a valid count."));
		return;
	}

	TArray<FAssetData> SelectedAssetDatas = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 SuccessCount = 0;

	for (const FAssetData& AssetData : SelectedAssetDatas)
	{
		for (int32 i = 0; i < Count; i++)
		{
			const FString SourceAssetPath = AssetData.ObjectPath.ToString();
			const FString TargetAssetName = AssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i);
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
		ShowNotifyInfo(TEXT("Successfully duplicated ") + FString::FromInt(SuccessCount) + " files.");
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
			ShowMessage(EAppMsgType::Ok, TEXT("Failed to find prefix for class ") +
												SelectedObject->GetClass()->GetName());
			continue;
		}

		FString OldName = SelectedObject->GetName();
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
		ShowNotifyInfo(TEXT("Successfully renamed ") + FString::FromInt(Counter) + TEXT(" assets."));
	}
}
