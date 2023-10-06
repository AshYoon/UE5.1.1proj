#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

class UItemBase;
/*this type of delegate will now will be valid for use */
DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

/*to describes add result */
UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added")

};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	/*Initialize*/
	FItemAddResult() :
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};



	//Actual amount of item that was added to inventory 
	UPROPERTY(BlueprintReadOnly , Category = "Item Add Result")
	int32 ActualAmountAdded;
	// Enum representing the end state of an add item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;
	/*ex) "inventory is full, information message that can be passed with the result  "*/
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;



	/* we're gonna call this directly,so we make this with static*/
	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;

		AddedNoneResult.ActualAmountAdded = 0;

		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;

		AddedNoneResult.ResultMessage = ErrorText;

		return AddedNoneResult;
	};

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;

		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;

		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;

		AddedPartialResult.ResultMessage = ErrorText;

		return AddedPartialResult;
	};

	static FItemAddResult AddedAll(const int32 AmountAdded,const FText& Message)
	{
		FItemAddResult AddedAllResult;

		AddedAllResult.ActualAmountAdded = AmountAdded;

		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;

		AddedAllResult.ResultMessage = Message;

		return AddedAllResult;
	};


};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U05_ACTION_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

		

public:

	//=========================================================================
	//                       PROPERTIES & VARIABLES
	//=========================================================================
	/* no public variable , */
	/* DELEGATE  SEND A  SIGNAL AND there's broadcaster and subscriber 
	broadcaster send signal -> anyclass that subcribes to it will then be notified and can take action */
	/* delegate instance*/
	FOnInventoryUpdated OnInventoryUpdated;

	//=========================================================================
	//                       FUNCTIONS 
	//=========================================================================
	UCInventoryComponent();

	UFUNCTION(Category = "Inventory")
	UItemBase* FindMathingItem(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemById(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

	/* Main function */
	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);

	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);

	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* ItemIn, const int32 AmointToSplit);

	//getters
	//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };
	//setters
	//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
	//UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };
	
	//UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };



	
private:



protected:

	//=========================================================================
	//                       PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;

	//migration for this 
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;
	/* in ue5 use TObjectPtr, UPROPERTY deosn't return Tobjectpointer */
	//UPROPERTY(VisibleAnywhere, Category = "Inventory")
	//TArray<TObjectptr<UItemBase>> InventoryContents;


	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;

	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	float InventoryWeightCapacity;
	
	UPROPERTY(EditInstanceOnly, Category = "InventroyIDETest")
	UItemBase* InventoryIDETest;
	//=========================================================================
	//                       FUNCTIONS 
	//=========================================================================



	virtual void BeginPlay() override;
	FItemAddResult HandleNonStackableItem(UItemBase* InputItem  );
	int32 HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CaculateWeightAddAmoint(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CaculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);




};
