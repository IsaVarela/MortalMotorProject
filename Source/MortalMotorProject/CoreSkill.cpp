// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreSkill.h"

const TArray<TSharedPtr<FCoreSkill>>& FCoreSkill::GetAllUpgrades() const
{
	return CoreSkillUpgrades;
}
