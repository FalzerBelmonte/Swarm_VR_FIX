// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "BT_A.generated.h"

UCLASS()
class ZDVR_02_API ABT_A : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABT_A();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet Variables")
		bool Active = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet Variables", meta = (ExposeOnSpawn = "true"))
		FVector SpawnerLocation = FVector(FVector::ZeroVector);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet Variables", meta = (ExposeOnSpawn = "true"))
		FVector Target = FVector(FVector::ZeroVector);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet Variables", meta = (ExposeOnSpawn = "true"))
		int ID = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet Variables", meta = (ExposeOnSpawn = "true"))
		FVector SpawnPos = FVector(FVector::ZeroVector);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet Variables", meta = (ExposeOnSpawn = "true"))
		float Speed = 8000.f;
	

	UFUNCTION(BlueprintCallable, Category = "Bullet Functions")
		FTransform MakeCollisionSize(FTransform BulletHead, float DeltaSec);
	
	UFUNCTION(BlueprintCallable, Category = "Bullet Functions")
		void Fire();

	UFUNCTION(BlueprintCallable, Category = "Bullet Functions")
		void DestroyBullet();

	
	float PlusDistance = 1.2f;
	
	void Move();
	void Awake();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	float DS = 0.f;
	FVector TargetPlus = FVector(FVector::ZeroVector);
	
	FVector temp_01 = FVector(FVector::ZeroVector);
	float temp_02 = 0.f;

};
