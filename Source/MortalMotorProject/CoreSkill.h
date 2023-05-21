// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreSkill.generated.h"


DECLARE_DELEGATE(FOnSkillAction);

USTRUCT(BlueprintType)
struct MORTALMOTORPROJECT_API FCoreSkill
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString SkillName;

	FOnSkillAction OnSkillActionDelegate;

	bool operator==(const FCoreSkill& Other) const
	{
		// Implement the equality check logic here
		return this->SkillName == Other.SkillName;
	}

	bool operator!=(const FCoreSkill& Other) const
	{
		// Implement the equality check logic here
		return this->SkillName != Other.SkillName;
	}

	~FCoreSkill();
};
