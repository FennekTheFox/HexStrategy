#pragma once

#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "Components/WidgetComponent.h"

#include "InteractionComponent.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class UInteraction : public UObject 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnExecuteInteraction(UInteractionComponent* Component);

	UWorld* GetWorld() const override;

};

//UCLASS(BlueprintType, HideCategories = (Transform, Materials, UserInterface, Interaction, Physics, Animation, Collision, HLOD, Rendering, Navigation, VirtualTexture, Cooking, Mobile, Material Parameters, Ray Tracing, TextureStreaming, AssetUserData), meta=(BlueprintSpawnableComponent)
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent), HideCategories = ("Transform", "Materials", "UserInterface", "Interaction", "Physics", "Animation", "Collision", "HLOD", "Rendering", "Navigation", "VirtualTexture", "Cooking", "Mobile", "Material Parameters", "Ray Tracing", "TextureStreaming", "AssetUserData"))
class UInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()

	UInteractionComponent();

public:
	UPROPERTY(BlueprintReadOnly, Instanced, EditAnywhere)
		UInteraction* Interaction;


	void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void TriggerInteraction();
	UFUNCTION(BlueprintCallable)
		void SetInteractionVisible(bool newVisible);
};