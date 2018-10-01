#pragma once

UENUM(BlueprintType)

enum class ERYULedgePosition3D : uint8
{
	NONE UMETA(DisplayName = "none"),
	Front_080cm UMETA(DisplayName = "FrontWall(0.8m)"),
	Front_100cm UMETA(DisplayName = "FrontWall(1.0m)"),
	Front_150cm UMETA(DisplayName = "FrontWall(1.5m)"),
	Front_200cm UMETA(DisplayName = "FrontWall(2.0m)"),
	Above_080cm UMETA(DisplayName = "AboveLedge(0.8m)"),
	Above_100cm UMETA(DisplayName = "AboveLedge(1.0m)"),
	Above_150cm UMETA(DisplayName = "AboveLedge(1.5m)"),
	Above_200cm UMETA(DisplayName = "AboveLedge(2.0m)"),
	Above_450cm UMETA(DisplayName = "AboveLedge(4.5m)"),
	Hurdle_080cm UMETA(DisplayName = "FrontHurdle(0.8m)"),

};