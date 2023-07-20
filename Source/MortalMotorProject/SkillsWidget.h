// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoreSkill.h"
#include "SkillsWidget.generated.h"

UCLASS()
class MORTALMOTORPROJECT_API USkillsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void BeginDestroy() override;
	

	UFUNCTION()
	void ActivateButton_1();

	UFUNCTION()
	void ActivateButton_2();

	UFUNCTION()
	void ActivateButton_3();

private:
	class AMortalMortarGameMode* GameMode;

	class UButton* m_Btn1; 
	class UTextBlock* m_Btn1Text;

	class UButton* m_Btn2;
	class UTextBlock* m_Btn2Text;

	class UButton* m_Btn3;
	class UTextBlock* m_Btn3Text;

	FOnSkillAction m_btn1Action;
	FOnSkillAction m_btn2Action;
	FOnSkillAction m_btn3Action;

	TArray<TSharedPtr<FCoreSkill>> m_Skills;

	void RestoreUnselectedSkills(FOnSkillAction& skillAction);
	void InitWidget();


	
public:
	UFUNCTION()
	void InitSkillChoices();
};
