// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "PlayerUI.h"
#include "PlayerMotorCar.generated.h"

// Delegate signature
DECLARE_DELEGATE_OneParam(FOnGoldCollected,float)

UCLASS()
class MORTALMOTORPROJECT_API APlayerMotorCar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	APlayerMotorCar();

	UFUNCTION(BlueprintImplementableEvent)
	void TestGoldFunc(int goldamount);

	FOnGoldCollected OnGoldCollectedDelegate;


protected:
	virtual void BeginPlay() override;

private:
	void HandleGoldCollected();

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int GoldAmount;

	UPlayerUI* PlayerUI;

	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlayerUI> WidgetObject;

	
};
