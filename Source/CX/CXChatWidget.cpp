#include "CXChatWidget.h"
#include "CXPlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UCXChatWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatCommitted);
}

void UCXChatWidget::AddMessage(const FString& Message)
{
	UTextBlock* MessageText = NewObject<UTextBlock>(this);
	MessageText->SetText(FText::FromString(Message));
	MessageText->SetAutoWrapText(true);

	ChatLog->AddChild(MessageText);
	ChatLog->ScrollToEnd();
}

void UCXChatWidget::FocusInput()
{
	APlayerController* PC = GetOwningPlayer();
	if (IsValid(PC) == false)
	{
		return;
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(ChatInput->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

void UCXChatWidget::OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && Text.IsEmptyOrWhitespace() == false)
	{
		if (ACXPlayerController* PC = GetOwningPlayer<ACXPlayerController>())
		{
			PC->ServerSendChat(Text.ToString());
		}
	}

	ChatInput->SetText(FText::GetEmpty());
	RestoreGameInput();
}

void UCXChatWidget::RestoreGameInput()
{
	APlayerController* PC = GetOwningPlayer();
	if (IsValid(PC) == false)
	{
		return;
	}

	PC->SetInputMode(FInputModeGameOnly());
	PC->bShowMouseCursor = false;
}
