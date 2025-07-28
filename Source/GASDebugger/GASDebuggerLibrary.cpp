// Fill out your copyright notice in the Description page of Project Settings.


#include "GASDebuggerLibrary.h"

#include "Kismet/KismetMathLibrary.h"


FString UGASDebuggerLibrary::GetTimeInfo()
{
	FString CurrentTime = FString::Printf(TEXT("%d:%d:%d:%d"), UKismetMathLibrary::Now().GetHour(),
				   UKismetMathLibrary::Now().GetMinute(),
				   UKismetMathLibrary::Now().GetSecond(),
				   UKismetMathLibrary::Now().GetMillisecond());
	return CurrentTime;
}

