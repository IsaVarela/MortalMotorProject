// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsWidget.h"
#include "Components/Button.h"
#include "MortalMortarGameMode.h"
#include "Components/TextBlock.h"

void USkillsWidget::NativeConstruct()
{
	InitWidget();
}

void USkillsWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

void USkillsWidget::InitWidget()
{
	GameMode = Cast<AMortalMortarGameMode>(GetWorld()->GetAuthGameMode());
	
	//---------------BUTTON 1------------------------------------
	UWidget* ButtonWidget_1 = GetWidgetFromName(TEXT("Btn1"));
	if (ButtonWidget_1)
	{
		m_Btn1 = Cast<UButton>(ButtonWidget_1);

		if (m_Btn1)
		{
			FScriptDelegate ScriptDelegate;
			ScriptDelegate.BindUFunction(this, FName("ActivateButton_1"));
			m_Btn1->OnClicked.Add(ScriptDelegate);
		}
	}

	//---------------BUTTON 2------------------------------------
	UWidget* ButtonWidget_2 = GetWidgetFromName(TEXT("Btn2"));
	if (ButtonWidget_2)
	{
		m_Btn2 = Cast<UButton>(ButtonWidget_2);

		if (m_Btn2)
		{
			FScriptDelegate ScriptDelegate;
			ScriptDelegate.BindUFunction(this, FName("ActivateButton_2"));
			m_Btn2->OnClicked.Add(ScriptDelegate);
		}
	}

	//---------------BUTTON 3------------------------------------
	UWidget* ButtonWidget_3 = GetWidgetFromName(TEXT("Btn3"));
	if (ButtonWidget_3)
	{
		m_Btn3 = Cast<UButton>(ButtonWidget_3);

		if (m_Btn3)
		{
			FScriptDelegate ScriptDelegate;
			ScriptDelegate.BindUFunction(this, FName("ActivateButton_3"));
			m_Btn3->OnClicked.Add(ScriptDelegate);
		}
	}

	//------------TEXT 1-----------------------------------------
	UWidget* TextWidget_1 = GetWidgetFromName(FName("Text1"));

	if (TextWidget_1)
	{
		m_Btn1Text = Cast<UTextBlock>(TextWidget_1);
	}

	//------------TEXT 2-----------------------------------------
	UWidget* TextWidget_2 = GetWidgetFromName(FName("Text2"));

	if (TextWidget_2)
	{
		m_Btn2Text = Cast<UTextBlock>(TextWidget_2);
	}

	//------------TEXT 3-----------------------------------------
	UWidget* TextWidget_3 = GetWidgetFromName(FName("Text3"));

	if (TextWidget_3)
	{
		m_Btn3Text = Cast<UTextBlock>(TextWidget_3);
	}
}

void USkillsWidget::ActivateButton_1()
{
	RestoreUnselectedSkills(m_btn1Action);
	m_btn1Action.ExecuteIfBound();
	
}

void USkillsWidget::ActivateButton_2()
{
	RestoreUnselectedSkills(m_btn2Action);
	m_btn2Action.ExecuteIfBound();
	
}

void USkillsWidget::ActivateButton_3()
{
	RestoreUnselectedSkills(m_btn3Action);
	m_btn3Action.ExecuteIfBound();
	
}

void USkillsWidget::RestoreUnselectedSkills(FOnSkillAction& skillAction)
{
	if (m_Skills.IsEmpty()) { return; }
	
	for (const auto ele : m_Skills)
	{
		if (!ele->bIsGenericSkill && ele->OnSkillActionDelegate.GetHandle() != skillAction.GetHandle())
		{
			GameMode->AddSkillsToPool(ele);
		}
	}

	m_Skills.Empty();
}

void USkillsWidget::InitSkillChoices()
{
	if (!GameMode) { return; }

	//for button 1
	auto temp = GameMode->GetRandomSkill();
	m_Skills.Add(temp);

	if (m_btn1Action.IsBound())
	{
		m_btn1Action.Unbind();
	}
	m_btn1Action = temp->OnSkillActionDelegate;
	m_Btn1Text->SetText(FText::FromString(temp->SkillName));

	//for button 2
	auto temp2 = GameMode->GetRandomSkill();
	m_Skills.Add(temp2);

	if (m_btn2Action.IsBound())
	{
		m_btn2Action.Unbind();
	}

	m_btn2Action = temp2->OnSkillActionDelegate;
	m_Btn2Text->SetText(FText::FromString(temp2->SkillName));

	//for button 3
	auto temp3 = GameMode->GetRandomSkill();
	m_Skills.Add(temp3);

	if (m_btn3Action.IsBound())
	{
		m_btn3Action.Unbind();
	}

	m_btn3Action = temp3->OnSkillActionDelegate;
	m_Btn3Text->SetText(FText::FromString(temp3->SkillName));

	
}
