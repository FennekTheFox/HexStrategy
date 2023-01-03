#pragma once

#include <Serialization/ObjectAndNameAsStringProxyArchive.h>
#include "ObjectRecord.generated.h"

USTRUCT(BlueprintType)
struct FObjectRecord
{

	GENERATED_BODY()

	FObjectRecord()
	: ClassName("")
	{}

public:

	// class that this object is
	UPROPERTY(BlueprintReadWrite)
		FString ClassName;
	UPROPERTY(BlueprintReadWrite)
		TArray<uint8> Data;

	static const FObjectRecord SerializeObject(UObject* Object)
	{
		if (Object)
		{
			FObjectRecord  Rec;
			UClass* ObjectClass = Object->GetClass();
			ObjectClass->GetName(Rec.ClassName);

			//uses memory WRITER to SERIALIZE the object and WRITE the data
			FMemoryWriter MemAr(Rec.Data);
			FObjectAndNameAsStringProxyArchive Writer(MemAr, false);
			ObjectClass->SerializeTaggedProperties(Writer, (uint8*)Object, ObjectClass, nullptr);

			return Rec;
		}
		else
		{
			return FObjectRecord();
		}
	}

	template<typename T>
	T* DeserializeObject(UObject* Outer) const 
	{
		//UE_LOG(LogTemp, Log, TEXT("Trying to find class of name \"%s\""), *ClassName);

		//Try to find the class (Q: what if the class hasnt been loaded yet?)
		if (UClass* ObjectClass = FindObject<UClass>(nullptr, *ClassName))
		{
			UObject* Out = NewObject<UObject>(Outer, ObjectClass);

			//uses memory READER to READ the data and DESERIALIZE it
			FMemoryReader MemAr(Data);
			FObjectAndNameAsStringProxyArchive Reader(MemAr, false);

			ObjectClass->SerializeTaggedProperties(Reader, (uint8*)Out, ObjectClass, nullptr);

			return Cast<T>(Out);
		}

		return nullptr;
	}
};