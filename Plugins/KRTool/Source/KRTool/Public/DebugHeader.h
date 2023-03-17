#pragma once

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

void PrintGame(const FString& Message, const FColor& Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, Message);
	}
}

void PrintConsole(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

/// <summary>
/// 弹出窗口显示消息
/// </summary>
/// <param name="MsgType">消息窗口的类型</param>
/// <param name="Message">消息</param>
/// <param name="bShowMsgAsWarning">是否使用 Warning 作为标题</param>
/// <returns>用户点击结果</returns>
EAppReturnType::Type ShowMessage(EAppMsgType::Type MsgType, const FString& Message, bool bShowMsgAsWarning = true)
{
	if (bShowMsgAsWarning)
	{
		FText MsgTitle = FText::FromString(TEXT("Warning"));
		return FMessageDialog::Open(MsgType, FText::FromString(Message), &MsgTitle);
	}
	else
	{
		return FMessageDialog::Open(MsgType, FText::FromString(Message));
	}
}

/// <summary>
/// 显示提示信息
/// </summary>
/// <param name="Message">消息</param>
void ShowNotifyInfo(const FString& Message)
{
	FNotificationInfo NotifyInfo(FText::FromString(Message));
	NotifyInfo.bUseLargeFont = true;
	NotifyInfo.FadeOutDuration = 5.0f;

	FSlateNotificationManager::Get().AddNotification(NotifyInfo);
}