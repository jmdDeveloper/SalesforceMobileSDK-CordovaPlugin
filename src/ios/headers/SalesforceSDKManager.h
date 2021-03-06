/*
 Copyright (c) 2014, salesforce.com, inc. All rights reserved.
 
 Redistribution and use of this software in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions
 and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of
 conditions and the following disclaimer in the documentation and/or other materials provided
 with the distribution.
 * Neither the name of salesforce.com, inc. nor the names of its contributors may be used to
 endorse or promote products derived from this software without specific prior written
 permission of salesforce.com, inc.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "SFAuthenticationManager.h"
#import "SalesforceSDKCoreDefines.h"

@class SFUserAccount, SFSDKAppConfig;

typedef NS_ENUM(NSUInteger, SFAppType) {
    kSFAppTypeNative,
    kSFAppTypeHybrid,
    kSFAppTypeReactNative
};


@protocol SalesforceSDKManagerDelegate <NSObject>

@optional

/**
 Called after UIApplicationWillResignActiveNotification is received
 */
- (void)sdkManagerWillResignActive;

/**
 Called after UIApplicationDidBecomeActiveNotification is received.
 */
- (void)sdkManagerDidBecomeActive;

/**
 Called after UIApplicationWillEnterForegroundNotification is received.
 */
- (void)sdkManagerWillEnterForeground;

/**
 Called after UIApplicationDidEnterBackgroundNotification is received
 */
- (void)sdkManagerDidEnterBackground;

@end

/**
 This class will manage the basic infrastructure of the Mobile SDK elements of the app,
 including the orchestration of authentication, passcode displaying, and management of app
 backgrounding and foregrounding state.
 */
@interface SalesforceSDKManager : NSObject <SFAuthenticationManagerDelegate>

/**
 The class instance to be used to instantiate the singleton.
 */
+ (void)setInstanceClass:(Class)className;

/**
 @return The singleton instance of the SDK Manager.
 */
+ (instancetype)sharedManager;

@property (nonatomic, strong) SFSDKAppConfig *appConfig;

/**
 Whether or not the SDK is currently in the middle of a launch process.
 */
@property (nonatomic, readonly) BOOL isLaunching;


/**
 App type (native, hybrid or react native)
 */
@property (nonatomic, readonly) SFAppType appType;

/**
 The Connected App ID configured for this application.
 */
@property (nonatomic, copy) NSString *connectedAppId;

/**
 The Connected App Callback URI configured for this application.
 */
@property (nonatomic, copy) NSString *connectedAppCallbackUri;

/**
 The OAuth scopes configured for this application.
 */
@property (nonatomic, strong) NSArray *authScopes;

/**
 Whether or not to attempt authentication as part of the launch process.  Default
 value is YES.
 */
@property (nonatomic, assign) BOOL authenticateAtLaunch;

/**
 The configured post launch action block to execute when launch completes.
 */
@property (nonatomic, copy) SFSDKPostLaunchCallbackBlock postLaunchAction;

/**
 The configured launch error action block to execute in the event of an error during launch.
 */
@property (nonatomic, copy) SFSDKLaunchErrorCallbackBlock launchErrorAction;

/**
 The post logout action block to execute after the current user has been logged out.
 */
@property (nonatomic, copy) SFSDKLogoutCallbackBlock postLogoutAction;

/**
 The switch user action block to execute when switching from one user to another.
 */
@property (nonatomic, copy) SFSDKSwitchUserCallbackBlock switchUserAction;

/**
 The block to execute after the app has entered the foreground.
 */
@property (nonatomic, copy) SFSDKAppForegroundCallbackBlock postAppForegroundAction;

/**
 Whether or not to use a security snapshot view when the app is backgrounded, to prevent
 sensitive data from being displayed outside of the app context.  Default is YES.
 */
@property (nonatomic, assign) BOOL useSnapshotView;

/**
 A custom view to use as the "image" that represents the app display when it is backgrounded.
 Default will be an opaque white view.
 */
@property (nonatomic, strong) UIView *snapshotView;

/**
 The preferred passcode provider for the app.  Defaults to kSFPasscodeProviderPBKDF2.
 NOTE: If you wanted to set your own provider, you could do the following:
         id<SFPasscodeProvider> *myProvider = [[MyProvider alloc] initWithProviderName:myProviderName];
         [SFPasscodeProviderManager addPasscodeProvider:myProvider];
         [SalesforceSDKManager setPreferredPasscodeProvider:myProviderName];
 */
@property (nonatomic, copy) NSString *preferredPasscodeProvider;

/**
 Gets or sets a block that will return a user agent string, created with an optional qualifier.
 Default implementation, when executed, will return a user agent of the form:
 SalesforceMobileSDK/3.0.0 iPhone OS/8.1 (iPad) AppName/AppVersion *Native or Hybrid with optional qualifier* *Web-based user agent string*
 */
@property (nonatomic, copy) SFSDKUserAgentCreationBlock userAgentString;

/**
 Launches the SDK.  This will verify an existing passcode the first time it runs, and attempt to
 authenticate if the current user is not already authenticated.  @see postLaunchAction, launchErrorAction,
 postLogoutAction, and switchUserAction for callbacks that can be set for handling post launch
 actions.
 @return YES if the launch successfully kicks off, NO if launch is already running.
 */
- (BOOL)launch;

/**
 Adds an SDK Manager delegate to the list of delegates.
 @param delegate The delegate to add.
 */
- (void)addDelegate:(id<SalesforceSDKManagerDelegate>)delegate;

/**
 Removes an SDK Manager delegate from the list of delegates.
 @param delegate The delegate to remove.
 */
- (void)removeDelegate:(id<SalesforceSDKManagerDelegate>)delegate;

/**
 @return A log-friendly string of the launch actions that were taken, given in postLaunchAction.
 */
+ (NSString *)launchActionsStringRepresentation:(SFSDKLaunchAction)launchActions;

@end
