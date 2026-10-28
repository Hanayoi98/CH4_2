#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "CXChatWidget.generated.h"

class UEditableTextBox;
class UScrollBox;

UCLASS(Abstract)
class CX_API UCXChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddMessage(const FString& Message);
	void FocusInput();

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	void RestoreGameInput();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatLog;
};
