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
/// ����������ʾ��Ϣ
/// </summary>
/// <param name="MsgType">��Ϣ���ڵ�����</param>
/// <param name="Message">��Ϣ</param>
/// <param name="bShowMsgAsWarning">�Ƿ�ʹ�� Warning ��Ϊ����</param>
/// <returns>�û�������</returns>
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
/// ��ʾ��ʾ��Ϣ
/// </summary>
/// <param name="Message">��Ϣ</param>
void ShowNotifyInfo(const FString& Message)
{
	FNotificationInfo NotifyInfo(FText::FromString(Message));
	NotifyInfo.bUseLargeFont = true;
	NotifyInfo.FadeOutDuration = 5.0f;

	FSlateNotificationManager::Get().AddNotification(NotifyInfo);
}