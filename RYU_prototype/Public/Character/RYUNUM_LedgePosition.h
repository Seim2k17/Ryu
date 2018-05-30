#pragma once

UENUM(BlueprintType)

enum class ERYULedgePosition : uint8
{
	FrontWall_05m UMETA(DisplayName = "FrontWall(0.5m)"),
	FrontWall_1m UMETA(DisplayName = "FrontWall(1.0m)"),
	FrontWall_2m UMETA(DisplayName = "FrontWall(2.0m)"),
	AboveWall_05m UMETA(DisplayName = "AboveWall(0.5m)"),
	AboveWall_1m UMETA(DisplayName = "AboveWall(1.0m)"),
	AboveWall_2m UMETA(DisplayName = "AboveWall(2.0m)"),

};