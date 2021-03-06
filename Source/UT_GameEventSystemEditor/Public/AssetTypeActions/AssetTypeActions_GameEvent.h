/************************************************************************/
/* UMBRA TOOLS                                                          */
/* Maxwell - Axel Clerget   											*/
/************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_GameEvent : public FAssetTypeActions_Base
{
public:

	/************************************************************************/
	/* METHODS                                                              */
	/************************************************************************/

	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual bool CanLocalize() const override;
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;

};
