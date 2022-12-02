#include "EditorFunctionLibrary.h"

#include <PaperSprite.h>

void UEditorFunctionLibrary::SetSpriteAdditionalTexture(UPaperSprite* Sprite, UTexture* AdditionalTexture, int32 Index)
{
	//The additional source textures array is protected, which sucks because this function
	//is meant to set bulk values in the editor... so we gotta get creative

	FProperty* _property = Sprite->GetClass()->FindPropertyByName(TEXT("AdditionalSourceTextures"));
	FArrayProperty* ArrayProperty = CastField<FArrayProperty>(_property);

	if (ensure(ArrayProperty && _property))
	{
		TArray<TObjectPtr<UTexture>>* AdditionalSourceTextures = ArrayProperty->ContainerPtrToValuePtr<TArray<TObjectPtr<UTexture>>>(Sprite);

		if (ensure(AdditionalSourceTextures))
		{
			//Make sure the 
			if (AdditionalSourceTextures->Num() <= Index)
			{
				AdditionalSourceTextures->SetNum(Index+1);
			}

			(*AdditionalSourceTextures)[Index] = AdditionalTexture;
		}
	}
}
