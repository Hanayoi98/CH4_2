#include "CXPlayerController.h"
#include "CXPlayerState.h"
#include "CXGameState.h"
#include "CXChatWidget.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "CXPawn.h"
#include "CXUI.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Net/UnrealNetwork.h"

void ACXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}
	
	VehicleUI = CreateWidget<UCXUI>(this, VehicleUIClass);

	check(VehicleUI);

	VehicleUI->AddToViewport();

	if (ChatWidgetClass)
	{
		ChatWidget = CreateWidget<UCXChatWidget>(this, ChatWidgetClass);
		ChatWidget->AddToViewport();
	}
}

void ACXPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(EKeys::Enter, IE_Pressed, this, &ACXPlayerController::FocusChat);
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);

		if (bUseSteeringWheelControls && SteeringWheelInputMappingContext)
		{
			Subsystem->AddMappingContext(SteeringWheelInputMappingContext, 1);
		}
	}
}

void ACXPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, VehiclePawn);
}

void ACXPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());

		if (const ACXPlayerState* PS = GetPlayerState<ACXPlayerState>())
		{
			VehicleUI->UpdateCoins(PS->GetCoins());
			VehicleUI->UpdateBonusSpeed(PS->GetBonusMaxSpeed());
		}

		if (const ACXGameState* CXGameState = GetWorld()->GetGameState<ACXGameState>())
		{
			VehicleUI->UpdateTime(CXGameState->GetRemainingTime());

			switch (CXGameState->GetCurrentState())
			{
			case ECXGameState::Waiting:
				VehicleUI->UpdateStatus(FText::FromString(TEXT("WAITING")));
				break;
			case ECXGameState::Playing:
				VehicleUI->UpdateStatus(FText::FromString(TEXT("PLAYING")));
				break;
			case ECXGameState::Finished:
				VehicleUI->UpdateStatus(FText::FromString(TEXT("FINISHED")));
				break;
			}

			if (CXGameState->GetCurrentState() == ECXGameState::Finished && bResultShown == false)
			{
				bResultShown = true;

				if (CXGameState->IsWinner(PlayerState) == false)
				{
					VehicleUI->ShowResult(FText::FromString(TEXT("LOSE")));
				}
				else if (CXGameState->GetWinners().Num() > 1)
				{
					VehicleUI->ShowResult(FText::FromString(TEXT("CO-WINNER")));
				}
				else
				{
					VehicleUI->ShowResult(FText::FromString(TEXT("WIN")));
				}
			}
		}
	}
}

void ACXPlayerController::FocusChat()
{
	if (IsValid(ChatWidget))
	{
		ChatWidget->FocusInput();
	}
}

void ACXPlayerController::ServerSendChat_Implementation(const FString& Message)
{
	const FString Trimmed = Message.TrimStartAndEnd();
	if (Trimmed.IsEmpty() || Trimmed.Len() > MaxChatLength)
	{
		return;
	}

	const APlayerState* SenderState = GetPlayerState<APlayerState>();
	if (IsValid(SenderState) == false)
	{
		return;
	}

	const FString Chat = FString::Printf(TEXT("%s : %s"), *SenderState->GetPlayerName(), *Trimmed);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ACXPlayerController* Receiver = Cast<ACXPlayerController>(It->Get()))
		{
			Receiver->ClientReceiveChat(Chat);
		}
	}
}

void ACXPlayerController::ClientReceiveChat_Implementation(const FString& Message)
{
	if (IsValid(ChatWidget))
	{
		ChatWidget->AddMessage(Message);
	}
}

void ACXPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	VehiclePawn = CastChecked<ACXPawn>(InPawn);
}
