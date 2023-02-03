#pragma once

#include "PaperZDAnimMapping.generated.h"

class UPaperZDAnimationSource;
class UPaperZDAnimSequence;
class UUserDefinedEnum;
class UEnum;

/*A class that maps a user defined enum type to animation sequences*/
UCLASS(BlueprintType)
class PAPERZD_API UPaperZDAnimMapping : public UObject
{
	GENERATED_BODY()

public:
	/*The enum type used for this mapping*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere);
		UEnum* Enum;
	/*The enum type used for this mapping*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere);
		TMap<FName, UPaperZDAnimSequence*> Mapping;

		void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	/* Animation source that owns this sequence. */
	UPROPERTY(VisibleAnywhere, AssetRegistrySearchable, Category = Animation)
		UPaperZDAnimationSource* AnimSource;

public:

	FORCEINLINE UPaperZDAnimationSource* GetAnimSource() const { return AnimSource; }

	/* Set the Animation source that owns this sequence. */
	void SetAnimSource(UPaperZDAnimationSource* InAnimSource);
	void SetEnum(UEnum* NewEnum);
private:
	void Refresh();
	bool IsValidKey(FName Key);
};