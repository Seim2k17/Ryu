#pragma once

UENUM(BlueprintType)

enum class ERYULedgePosition : uint8
{
	Front_080cm UMETA(DisplayName = "FrontWall(0.8m)"),
	Front_100cm UMETA(DisplayName = "FrontWall(1.0m)"),
	Front_150cm UMETA(DisplayName = "FrontWall(1.5m)"),
	Front_200cm UMETA(DisplayName = "FrontWall(2.0m)"),
	Above_080cm UMETA(DisplayName = "AboveLedge(0.8m)"),
	Above_100cm UMETA(DisplayName = "AboveLedge(1.0m)"),
	Above_150cm UMETA(DisplayName = "AboveLedge(1.5m)"),
	Above_200cm UMETA(DisplayName = "AboveLedge(2.0m)"),

};