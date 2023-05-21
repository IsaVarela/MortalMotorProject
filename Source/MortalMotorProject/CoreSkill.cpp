// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreSkill.h"

FCoreSkill::~FCoreSkill()
{
	OnSkillActionDelegate.Unbind();
}
