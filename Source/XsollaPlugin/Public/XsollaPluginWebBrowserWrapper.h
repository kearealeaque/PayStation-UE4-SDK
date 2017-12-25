#pragma once

#include "Runtime/UMG/Public/UMG.h"
#include "Slate/SlateGameResources.h"
#include "XsollaPluginHttpTool.h"
#include "XsollaPluginTransactionDetails.h"

#include "XsollaPluginWebBrowserWrapper.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPaymantSucceeded, int32, Num, FTransactionDetails, transactionDetails);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPaymantCanceled, int32, Num);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPaymantFailed, FText, ErrorText, int32, ErrorCode);

class IWebBrowserWindow;

UCLASS()
class XSOLLAPLUGIN_API UXsollaPluginWebBrowserWrapper : public UUserWidget
{
    GENERATED_UCLASS_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUrlChanged, const FText&, Text);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadCompleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadError);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseWindow);

    virtual void NativeConstruct() override;

    void LoadURL(FString NewURL);
    void SetExternalId(FString str) { ExternalId = str; }
    void SetShopUrl(FString str) { ShopUrl = str; }
    void SetBrowserSize(float w, float h) 
    { 
        ContentSize.X = w;
        ContentSize.Y = h;
    }

public:
    UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
        FOnUrlChanged OnUrlChanged;

    UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
        FOnLoadCompleted OnLoadCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
        FOnLoadError OnLoadError;

    UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
        FOnCloseWindow OnCloseWindow;

    // shop delegates
    FOnPaymantSucceeded OnSucceeded;
    FOnPaymantCanceled OnCanceled;
    FOnPaymantFailed OnFailed;

private:
    void LoadSlateResources();
    void ComposeShopWrapper();
    void CloseShop();

    void HandleOnUrlChanged(const FText& Text);
    void HandleOnLoadCompleted();
    void HandleOnLoadError();
    bool HandleOnCloseWindow(const TWeakPtr<IWebBrowserWindow>& BrowserWindow);

    void OnTransactionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
    TSharedPtr<class SVerticalBox>		MainContent;
    TSharedPtr<class SWebBrowser>		WebBrowserWidget;
    TSharedPtr<class SSpinningImage>	SpinnerImage;
    TSharedPtr<class SButton>			CloseButton;
    TSharedPtr<class SButton>			HomeButton;
    TSharedPtr<class SVerticalBox>		Background;

    const FSlateBrush*                  SlateCloseBrush;
    const FSlateBrush*                  SlateBackBrush;
    const FSlateBrush*                  SlateSpinnerBrush;

    SHorizontalBox::FSlot& BrowserSlot				= SHorizontalBox::Slot();
    SHorizontalBox::FSlot& BrowserSlotMarginLeft	= SHorizontalBox::Slot();
    SHorizontalBox::FSlot& BrowserSlotMarginRight	= SHorizontalBox::Slot();

    float		ButtonSize;
    FVector2D	ViewportSize;
    FVector2D	ContentSize;
    FString		ShopSize;

    FString		InitialURL = "";
    FString		ShopUrl;
    bool		bSupportsTransparency = true;

    FString		ExternalId;
};