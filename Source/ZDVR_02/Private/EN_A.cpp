// Fill out your copyright notice in the Description page of Project Settings.


#include "EN_A.h"

// Sets default values
AEN_A::AEN_A() { PrimaryActorTick.bCanEverTick = true; }


// Called when the game starts or when spawned
void AEN_A::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEN_A::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DS = NoiseSpeed * DeltaTime;
	if (Active && unPause)	Move();
	if (isDead && unPause)	EndPath();
	SetActorLocation(temp_vec_01);

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void AEN_A::Move(){

	temp_vec_01 = GetActorLocation();
	SetActorRotation(FRotationMatrix::MakeFromX(FVector(0.f, 0.f, 120.f) - temp_vec_01).Rotator());
	temp_vec_01 += (DS * (temp_PerformanceSave - temp_vec_01).GetSafeNormal());
	
	if ((FVector::Distance(temp_PerformanceSave, temp_vec_01)) <= DS) {
		StepsIterator++;
		if (StepsIterator > Path_Steps) { 
			isDead = true; 
		}else {
			NoiseMaker(true, FMath::FRandRange((NoiseRange * -1), NoiseRange));
			temp_PerformanceSave = Targets[StepsIterator];
		}
		
	}	

}


void AEN_A::NoiseMaker(bool ALL, float RNG) {

	if (ALL) {
		temp_vec_04 = ((Targets[StepsIterator]).RotateAngleAxis(RNG, (FVector(0.f, 0.f, Targets[StepsIterator].Z)).GetSafeNormal()));
		//LastTarget = temp_PerformanceSave;
		NoiseSpeed = (FVector::Dist(temp_vec_04, temp_PerformanceSave) / (FVector::Distance(Targets[1], Targets[2]))) * Speed; 
		// Reference Distance -> FVector::Distance(Targets[1], Targets[2]);
		
		if (AllowDebug) UKismetSystemLibrary::DrawDebugLine(this, Targets[StepsIterator], temp_vec_04, FLinearColor::Red, DebugRenderTime, 6.f);
		
		if (StepsIterator == Path_Steps) temp_vec_04 = Targets[Path_Steps]; // last destination
		Targets[StepsIterator] = temp_vec_04;
		
		if (AllowDebug) UKismetSystemLibrary::DrawDebugSphere(this, Targets[StepsIterator], 100.f, 12, FLinearColor::Red, DebugRenderTime, 6.f);
	}
	// else -> run % on enemy id and select some 
}


void AEN_A::PathMaker(float path_LastStepDistance, float path_wdRng) {

	temp_flt_01 = FVector::Distance(StartLocation, TargetLocation);
	temp_vec_01 = (TargetLocation - StartLocation).GetSafeNormal();

	for (int i = 0; i < Path_Steps; i++) {
		temp_vec_04 = ((temp_flt_01 / Path_Steps) * i) * temp_vec_01;
		temp_flt_02 = FVector::Distance(temp_vec_01, temp_vec_04);
		temp_vec_04 += StartLocation;
		temp_flt_02 = UKismetMathLibrary::MapRangeClamped(temp_flt_02, 0.f, temp_flt_01, 0.f, 180.f);
		temp_vec_04.Z += FMath::Sin(FMath::DegreesToRadians(temp_flt_02)) * path_wdRng;
		Targets[i] = temp_vec_04;

		if (AllowDebug) UKismetSystemLibrary::DrawDebugSphere(this, Targets[i], 100.f, 12, FLinearColor::Black, DebugRenderTime, 4.f);
	}
	
	// Last step distance -> move directly to player location 
	temp_vec_04 = Targets[Path_Steps - 1];
	temp_vec_04 += (FRotationMatrix::MakeFromX(FVector(0.f, 0.f, 160.f) - temp_vec_04).Rotator()).Vector() * path_LastStepDistance;
	
	Targets[Path_Steps] = temp_vec_04;
	
	if (AllowDebug) UKismetSystemLibrary::DrawDebugSphere(this, Targets[Path_Steps], 100.f, 12, FLinearColor::White, DebugRenderTime, 2.f);

}


void AEN_A::Awake(bool debugAllow, bool park, float debugRTime, float spd, int path_stps, float path_ClosestDist, float path_WideRang, float noiseRng){

	isDead = false;
	Health = 100.f;
	Path_Steps = path_stps;
	for (int index = 0; index < (Path_Steps + 4); index++) Targets.Add(FVector(0.f, 0.f, 100 * index));

	AllowDebug = debugAllow;
	DebugRenderTime = debugRTime;

	Speed = spd;
	SpeedRefrence = Speed;
	NoiseSpeed = Speed;
	NoiseRange = noiseRng;

	temp_flt_01 = FVector::Distance(TargetLocation, FVector(0.f, 0.f, 100.f)) * path_ClosestDist;
	// Last step distance -> move directly to player location
	// path_ClosestDist -> min dist between player and enemy at the end of path
	PathMaker(temp_flt_01, path_WideRang);

	// setup 
	StepsIterator = 2;
	temp_PerformanceSave = Targets[StepsIterator];
	SetActorLocation(temp_PerformanceSave);
	//SetActorRotation(FRotationMatrix::MakeFromX(temp_PerformanceSave - FVector(0.f, 0.f, 120.f)).Rotator());
	//UE_LOG(LogTemp, Warning, TEXT("  -- Destroy ---ID: %d  "), EnemyID);
	if (park) {
		onSpawnPark = true;
		Park();
	}
	unPause = !park;
	Active = true;
}


bool AEN_A::CheckHealth(float DMG) {
	Health -= DMG;
	CheckSpeed();
	return ( (Health <= 0) ? true : false );
}

void AEN_A::CheckSpeed() {
	Speed = (Health / 100) * SpeedRefrence;
	NoiseSpeed = (Health / 100) * NoiseSpeed;
	// min speed 
	Speed += 120.f;
	NoiseSpeed += 120.f;
}

void AEN_A::Park(){
	temp_vec_01 = FVector(
		int(fmod(EnemyID, 10.f) * 800.f) + -2000.f 
		,(int(EnemyID/10.f) * 800.f) + -2000.f
		, -800.f
		);
	SetActorLocation(temp_vec_01);

	//UE_LOG(LogTemp, Warning, TEXT("  -- Destroy ---ID: %s  "), ToString(temp_vec_01));
	
}

float AEN_A::GetHealth(){ return Health; }
void AEN_A::ActivateEnemy() { Active = true; }
void AEN_A::PauseEnemy(bool pause){ 
	
	if (onSpawnPark) {
		onSpawnPark = false;
		temp_PerformanceSave = Targets[StepsIterator];
		temp_vec_01 = temp_PerformanceSave;
		SetActorLocation(temp_vec_01);
		//UE_LOG(LogTemp, Warning, TEXT("  -- Destroy ---  ---ID: %d  "), StepsIterator);
	}
	unPause = !pause; 
}
void AEN_A::DestroyEnemy() { 
	Active = false;
	isDead = false;
	Park();
	//this->Destroy(); 
}



// arrays https://www.youtube.com/watch?v=9sPWVJKYJ_0
// TArray<ASomeActor*> SomeActors;
// if (SomeActors.IsValidIndex(0))
// call BP event from c++ https://answers.unrealengine.com/questions/116529/view.html
// discord https://couchlearn.com/how-to-use-discord-rich-presence-in-unreal-engine-4/#comment-459
// Tarray opt https://www.unrealengine.com/en-US/blog/optimizing-tarray-usage-for-performance
// https://docs.unrealengine.com/en-US/Programming/UnrealArchitecture/TArrays/index.html

		//UE_LOG(LogTemp, Warning, TEXT(" temp_11: %s"), *temp_11.ToString());
		//UE_LOG(LogTemp, Warning, TEXT(" temp_11.Z: %f"), temp_11.Z);
			//UE_LOG(LogTemp, Warning, TEXT(" ############################### "));
			//UE_LOG(LogTemp, Warning, TEXT(" ###############################K BB: %s"), *Targets[StepsIterator].ToString());
		//GEngine->AddOnScreenDebugMessage(0, 30.f, FColor::Yellow," --- \n ");
		//GEngine->AddOnScreenDebugMessage(0, 30.f, FColor::Red, FString::SanitizeFloat(FMath::DegreesToRadians(temp_12)) + " --- /n" );
		//GEngine->AddOnScreenDebugMessage(0, 30.f, FColor::Red, FString::SanitizeFloat(FMath::RadiansToDegrees(temp_12)));

		//UE_LOG(LogTemp, Warning, TEXT("  DegreesToRadians  %f"), FMath::Sin(FMath::DegreesToRadians(45)));
		//UE_LOG(LogTemp, Warning, TEXT("  RadiansToDegrees %f"), FMath::Sin(FMath::DegreesToRadians(temp_12)));
			//UE_LOG(LogTemp, Warning, TEXT("  -- Destroy ---ID: %d  "), EnemyID);

// MOD  https://forums.unrealengine.com/development-discussion/blueprint-visual-scripting/1382311-modulo




