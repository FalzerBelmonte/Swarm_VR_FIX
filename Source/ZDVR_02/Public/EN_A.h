// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EN_A.generated.h"

UCLASS()
class ZDVR_02_API AEN_A : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEN_A();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Variables")
		bool Active = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Variables", meta = (ExposeOnSpawn = "true"))
		int EnemyID = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Variables", meta = (ExposeOnSpawn = "true"))
		FName EName= " --- ";
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Variables", meta = (ExposeOnSpawn = "true"))
		FVector TargetLocation = FVector(5000.f, 0.f, 160.f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Variables", meta = (ExposeOnSpawn = "true"))
		FVector StartLocation = FVector(0.f, 0.f, 3200.f);


	UFUNCTION(BlueprintCallable, Category = "Enemy Functions")
		void Awake(bool debugAllow, bool park = false , float debugRTime = 60.f, float spd = 120.f, int path_stps = 8, float path_ClosestDist = 0.75f, float path_WideRang = 360.f, float noiseRng = 30.f);
	
	UFUNCTION(BlueprintCallable, Category = "Enemy Functions")
		bool CheckHealth(float DMG);

	UFUNCTION(BlueprintPure, Category = "Enemy Functions")
		float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Enemy Functions")
		void ActivateEnemy();		
	
	UFUNCTION(BlueprintCallable, Category = "Enemy Functions")
		void PauseEnemy(bool pause);	
	
	UFUNCTION(BlueprintCallable, Category = "Enemy Functions")
		void DestroyEnemy();

	
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Functions")
		void EndPath();
	

	bool AllowDebug = false;
	bool onSpawnPark = false;
	int Path_Steps = 8;
	float DebugRenderTime = 60.f;
	float Health = 100.f;
	float Speed = 120.f;
	float NoiseRange = 30.f;

	void PathMaker(float path_LastStepDistance, float path_wdRng);
	void NoiseMaker(bool ALL, float RNG);
	void CheckSpeed(); 
	void Move();
	void Park();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame //
	virtual void Tick(float DeltaTime) override;

private:

	bool isDead = false;
	bool unPause = false;
	
	int StepsIterator = 0;
	int Path_MoveSteps = 0;

	float DS = 0.f;
	float NoiseSpeed = 0.f;
	float SpeedRefrence = 0.f;

	FVector temp_PerformanceSave = FVector(FVector::ZeroVector);

	TArray <FVector> Targets;

	// temp variabbles
	FVector temp_vec_01 = FVector(FVector::ZeroVector);
	FVector temp_vec_04 = FVector(FVector::ZeroVector);
	float temp_flt_01 = 0.f;
	float temp_flt_02 = 0.f;

};

