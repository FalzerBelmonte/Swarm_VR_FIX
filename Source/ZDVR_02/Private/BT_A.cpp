// Fill out your copyright notice in the Description page of Project Settings.

#include "BT_A.h"

// Sets default values
ABT_A::ABT_A() { PrimaryActorTick.bCanEverTick = true; }

// Called when the game starts or when spawned
void ABT_A::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABT_A::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DS = Speed * DeltaTime;
	if (Active)		Move();

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void ABT_A::Move()
{
	temp_01 = GetActorLocation();
	SetActorRotation(FRotationMatrix::MakeFromX(TargetPlus - temp_01).Rotator());
	temp_01 += (DS * (TargetPlus - temp_01).GetSafeNormal());
	(FVector::Distance(TargetPlus, temp_01)) <= DS ? DestroyBullet() : SetActorLocation(temp_01);
	if (temp_01.Z < 0.f) DestroyBullet(); // save performance on shooting surface

}

FTransform ABT_A::MakeCollisionSize(FTransform BulletHead, float DeltaSec)
{
	Awake();
	
	temp_01 = UKismetMathLibrary::GetDirectionUnitVector(Target, GetActorLocation());
	temp_01 = FVector(
		FVector(BulletHead.GetLocation()).X + ( (DeltaSec * Speed / 2) + (temp_01.X * .082) ),
		(BulletHead.GetLocation()).Y,
		(BulletHead.GetLocation()).Z
	);
	
	return FTransform(
		BulletHead.GetRotation(),
		temp_01,
		FVector(((DeltaSec * Speed) / 60),
			(BulletHead.GetScale3D()).Y,
			(BulletHead.GetScale3D()).Z)
	);
		
}

void ABT_A::Awake(){
	
	SetActorLocation(SpawnPos);
	SetActorRotation(FRotationMatrix::MakeFromX(Target - SpawnPos).Rotator());
	temp_02 = (FVector::Distance(GetActorLocation(), Target)) * PlusDistance ;
	temp_01 = (Target - GetActorLocation()).GetSafeNormal();
	TargetPlus = (temp_01 * temp_02) + GetActorLocation();
}

void ABT_A::Fire() { Active = true; }
void ABT_A::DestroyBullet()	{ this->Destroy(); }



// c++ FTransform
// https://cpp.hotexamples.com/examples/-/FTransform/-/cpp-ftransform-class-examples.html
// move actor AddActorLocalOffset
//UE_LOG(LogTemp, Warning, TEXT("----------------------I just started running"));

//https://unrealcpp.com/debug-logging/
//https://docs.unrealengine.com/en-US/PythonAPI/class/Vector.html
//https://www.unrealengine.com/en-US/blog/ranged-based-for-loops





