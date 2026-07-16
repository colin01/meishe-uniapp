#import "OSXMeisheVideoModule.h"

#import <AVFoundation/AVFoundation.h>
#import <math.h>
#import <MobileCoreServices/MobileCoreServices.h>
#import <UIKit/UIKit.h>
#import <NvShortVideoCore/NvShortVideoCore-Swift.h>
#import <NvStreamingSdkCore/NvstreamingSdkCore.h>

@interface OSXMeisheVideoModule () <UIImagePickerControllerDelegate, UINavigationControllerDelegate, NvModuleManagerDelegate, NvModuleManagerCompileStateDelegate>

@property (nonatomic, copy) UniModuleKeepAliveCallback sessionCallback;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, assign) NSTimeInterval maxTime;
@property (nonatomic, assign) BOOL publishMode;
@property (nonatomic, assign) BOOL exporting;
@property (nonatomic, assign) BOOL completingSuccess;
@property (nonatomic, assign) BOOL sessionFinished;
@property (nonatomic, strong) NvModuleManager *moduleManager;
@property (nonatomic, strong) UIImagePickerController *picker;
@property (nonatomic, copy) NSString *reeditProjectId;
@property (nonatomic, weak) UIViewController *reeditPresenter;
@property (nonatomic, weak) DCUniSDKInstance *observedInstance;
@property (nonatomic, assign) BOOL observingReeditPresentation;
@property (nonatomic, assign) BOOL observingInstanceState;

@end

@implementation OSXMeisheVideoModule

// Keep the module alive until its only terminal callback has been delivered.
static OSXMeisheVideoModule *OSXMeisheActiveModule;
static BOOL OSXMeisheLicenseVerified;
static void *OSXMeisheReeditPresentationContext = &OSXMeisheReeditPresentationContext;
static void *OSXMeisheInstanceStateContext = &OSXMeisheInstanceStateContext;

UNI_EXPORT_METHOD(@selector(start:callback:))
UNI_EXPORT_METHOD(@selector(editor:callback:))
UNI_EXPORT_METHOD(@selector(shooting:callback:))

- (void)start:(NSDictionary *)options callback:(UniModuleKeepAliveCallback)callback
{
    [self openWithOptions:options callback:callback forcedMode:nil];
}

- (void)editor:(NSDictionary *)options callback:(UniModuleKeepAliveCallback)callback
{
    [self openWithOptions:options callback:callback forcedMode:@"edit"];
}

- (void)shooting:(NSDictionary *)options callback:(UniModuleKeepAliveCallback)callback
{
    // Keep the historical API callable without advertising a capture feature the plugin does not ship.
    [self invoke:callback result:@{ @"errorCode": @"unsupported", @"errorMessage": @"当前插件未提供独立拍摄入口" }];
}

- (void)openWithOptions:(NSDictionary *)options
               callback:(UniModuleKeepAliveCallback)callback
             forcedMode:(NSString *)forcedMode
{
    if (![options isKindOfClass:[NSDictionary class]]) {
        [self invoke:callback result:@{ @"errorCode": @"invalid_options", @"errorMessage": @"缺少插件参数" }];
        return;
    }
    BOOL isPublish = [[self stringValue:options[@"mode"]] caseInsensitiveCompare:@"publish"] == NSOrderedSame;
    BOOL missingMaxTime = options[@"maxTime"] == nil;
    if ((forcedMode.length == 0 && (!isPublish || missingMaxTime))
            || (forcedMode.length > 0 && missingMaxTime)) {
        [self invoke:callback result:@{ @"errorCode": @"invalid_options", @"errorMessage": @"调用必须传入 maxTime；start 调用还必须传入 mode: 'publish'" }];
        return;
    }
    if (![self beginSession:options callback:callback forcedMode:forcedMode]) {
        return;
    }
    if (self.sessionFinished) {
        return;
    }
    if (![self verifyLicense]) {
        [self finishFailure:@"license_invalid" message:@"美摄授权文件校验失败"];
        return;
    }

    self.moduleManager = [NvModuleManager sharedInstance];
    self.moduleManager.delegate = self;
    self.moduleManager.compileDelegate = self;
    [self.moduleManager prepareDownloadFolders];

    if (self.publishMode) {
        [self presentPublishPicker];
    } else {
        [self presentEditorWithConfig:[self editorConfig] completion:^(BOOL finished) {
            if (!finished && !self.exporting && !self.completingSuccess) {
                [self finishCancelled];
            }
        }];
    }
}

- (BOOL)beginSession:(NSDictionary *)options
            callback:(UniModuleKeepAliveCallback)callback
          forcedMode:(NSString *)forcedMode
{
    @synchronized (OSXMeisheVideoModule.class) {
        if (OSXMeisheActiveModule && !OSXMeisheActiveModule.sessionFinished) {
            [self invoke:callback result:@{ @"errorCode": @"session_busy", @"errorMessage": @"已有视频编辑会话正在进行" }];
            return NO;
        }
        OSXMeisheActiveModule = self;
        self.sessionFinished = NO;
        self.sessionCallback = callback;
        NSString *providedSessionId = [self stringValue:options[@"sessionId"]];
        self.sessionId = providedSessionId.length > 0 ? providedSessionId : NSUUID.UUID.UUIDString;
        NSString *mode = forcedMode.length > 0 ? forcedMode : [self stringValue:options[@"mode"]];
        self.publishMode = [mode caseInsensitiveCompare:@"publish"] == NSOrderedSame;
        self.maxTime = MAX(0, [self numberValue:options[@"maxTime"]]);
        self.exporting = NO;
        self.completingSuccess = NO;
        self.reeditProjectId = nil;
        [self watchInstanceDestruction];
        return YES;
    }
}

- (BOOL)verifyLicense
{
    @synchronized (OSXMeisheVideoModule.class) {
        if (OSXMeisheLicenseVerified) return YES;
        NSBundle *mainBundle = NSBundle.mainBundle;
        NSArray<NSString *> *licensePaths = @[
            [mainBundle pathForResource:@"meishesdk" ofType:@"lic"] ?: @"",
            [mainBundle pathForResource:@"meishesdk" ofType:@"lic" inDirectory:@"license.bundle/license"] ?: @"",
            [mainBundle pathForResource:@"meishesdk" ofType:@"lic" inDirectory:@"BundleResources/license.bundle/license"] ?: @"",
            [[mainBundle pathForResource:@"license" ofType:@"bundle"] stringByAppendingPathComponent:@"license/meishesdk.lic"] ?: @""
        ];
        NSString *licensePath = nil;
        for (NSString *candidate in licensePaths) {
            if ([[NSFileManager defaultManager] fileExistsAtPath:candidate]) {
                licensePath = candidate;
                break;
            }
        }
        if (licensePath.length == 0 || ![[NSFileManager defaultManager] fileExistsAtPath:licensePath]) {
            return NO;
        }
        OSXMeisheLicenseVerified = [NvsStreamingContext verifySdkLicenseFile:licensePath];
        return OSXMeisheLicenseVerified;
    }
}

- (NvVideoConfig *)editorConfig
{
    NvVideoConfig *config = [NvVideoConfig new];
    config.primaryColor = [UIColor colorWithRed:252.0 / 255.0 green:62.0 / 255.0 blue:90.0 / 255.0 alpha:1.0];
    config.backgroundColor = UIColor.blackColor;
    config.panelBackgroundColor = [UIColor colorWithWhite:28.0 / 255.0 alpha:1.0];
    config.textColor = UIColor.whiteColor;
    config.secondaryTextColor = [UIColor colorWithRed:108.0 / 255.0 green:108.0 / 255.0 blue:119.0 / 255.0 alpha:1.0];
    config.albumConfig.type = MeicamMediaTypeVideo;
    config.albumConfig.maxSelectCount = self.publishMode ? 1 : 9;
    config.editConfig.editModeSource = NvEditModeSourceFirstAsset;
    config.compileConfig.resolution = NvVideoCompileResolutionResolution720;
    return config;
}

- (void)presentPublishPicker
{
    UIViewController *presenter = [self presenter];
    if (!presenter) {
        [self finishFailure:@"presenter_missing" message:@"无法打开视频选择器"];
        return;
    }
    UIImagePickerController *picker = [UIImagePickerController new];
    picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    picker.mediaTypes = @[ (NSString *)kUTTypeMovie ];
    picker.videoQuality = UIImagePickerControllerQualityTypeHigh;
    picker.delegate = self;
    self.picker = picker;
    [presenter presentViewController:picker animated:YES completion:nil];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:^{
        self.picker = nil;
        [self finishCancelled];
    }];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<UIImagePickerControllerInfoKey,id> *)info
{
    NSString *mediaType = info[UIImagePickerControllerMediaType];
    NSURL *url = info[UIImagePickerControllerMediaURL];
    if (![mediaType isEqualToString:(NSString *)kUTTypeMovie] || !url.isFileURL) {
        [picker dismissViewControllerAnimated:YES completion:^{
            self.picker = nil;
            [self showInvalidSelection];
        }];
        return;
    }
    [picker dismissViewControllerAnimated:YES completion:^{
        self.picker = nil;
        NSString *cachedPath = [self copyVideoToCache:url];
        if (cachedPath.length == 0) {
            [self finishFailure:@"source_prepare_failed" message:@"视频文件准备失败"];
            return;
        }
        NSTimeInterval duration = [self videoDuration:cachedPath];
        if (duration <= 0) {
            [self finishFailure:@"invalid_video" message:@"所选文件不是可编辑的视频"];
            return;
        }
        if (self.maxTime > 0 && duration > self.maxTime) {
            [self presentOvertimeAlertForVideo:cachedPath];
            return;
        }
        [self finishSuccess:cachedPath];
    }];
}

- (void)showInvalidSelection
{
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"无法使用该文件"
                                                                   message:@"请选择单个视频文件"
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"重新选择" style:UIAlertActionStyleDefault handler:^(__unused UIAlertAction *action) {
        [self reopenPublishPickerAfterAlertDismissal];
    }]];
    [[self presenter] presentViewController:alert animated:YES completion:nil];
}

- (void)presentOvertimeAlertForVideo:(NSString *)videoPath
{
    NSString *message = [NSString stringWithFormat:@"视频时长超过 %.0f 秒限制，可裁剪后继续发布", self.maxTime];
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"视频时长超限"
                                                                   message:message
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"重新选择" style:UIAlertActionStyleDefault handler:^(__unused UIAlertAction *action) {
        [self reopenPublishPickerAfterAlertDismissal];
    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"进入编辑" style:UIAlertActionStyleDefault handler:^(__unused UIAlertAction *action) {
        // UIAlertController dismisses after its action handler. Defer one main-loop turn so the SDK
        // receives the real page controller rather than the disappearing alert controller.
        dispatch_async(dispatch_get_main_queue(), ^{
            [self openSelectedVideoForEdit:videoPath];
        });
    }]];
    [[self presenter] presentViewController:alert animated:YES completion:nil];
}

- (void)reopenPublishPickerAfterAlertDismissal
{
    // UIAlertController dismisses after its action handler. Present from the page on the next run loop.
    dispatch_async(dispatch_get_main_queue(), ^{
        if (!self.sessionFinished) {
            [self presentPublishPicker];
        }
    });
}

- (void)openSelectedVideoForEdit:(NSString *)videoPath
{
    NvEditFileItem *item = [NvEditFileItem new];
    item.filePath = videoPath;
    item.speed = 1.0;
    item.duration = (int64_t)([self videoDuration:videoPath] * 1000000.0);
    NSString *projectId = NSUUID.UUID.UUIDString;
    [NvEditManager prepareProEditEnv];
    if (![NvEditManager newProjectWithFiles:@[ item ] music:nil projectId:projectId]) {
        [self finishFailure:@"project_create_failed" message:@"无法创建视频编辑项目"];
        return;
    }
    NvEditProjectInfo *project = [NvModuleManager projectInfoForProject:projectId];
    UIViewController *presenter = [self presenter];
    if (!project || !presenter) {
        [self finishFailure:@"project_open_failed" message:@"无法打开视频编辑器"];
        return;
    }
    self.reeditProjectId = projectId;
    [self watchReeditDismissalFromPresenter:presenter];
    [self.moduleManager reeditProject:project presentViewController:presenter config:[self editorConfig]];
}

- (void)watchReeditDismissalFromPresenter:(UIViewController *)presenter
{
    [self stopWatchingReeditDismissal];
    self.reeditPresenter = presenter;
    [presenter addObserver:self
                forKeyPath:@"presentedViewController"
                   options:NSKeyValueObservingOptionNew
                   context:OSXMeisheReeditPresentationContext];
    self.observingReeditPresentation = YES;
}

- (void)presentEditorWithConfig:(NvVideoConfig *)config completion:(void (^)(BOOL finished))completion
{
    UIViewController *presenter = [self presenter];
    if (!presenter) {
        [self finishFailure:@"presenter_missing" message:@"无法打开视频编辑器"];
        return;
    }
    [self.moduleManager startEditWithPresent:presenter config:config with:completion];
}

- (void)watchInstanceDestruction
{
    [self stopWatchingInstanceDestruction];
    DCUniSDKInstance *instance = self.uniInstance;
    if (!instance) return;
    if (instance.uniState == DCUniInstanceDestroy) {
        [self finishCancelled];
        return;
    }
    self.observedInstance = instance;
    [instance addObserver:self
                forKeyPath:@"uniState"
                   options:NSKeyValueObservingOptionNew
                   context:OSXMeisheInstanceStateContext];
    self.observingInstanceState = YES;
}

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary<NSKeyValueChangeKey,id> *)change
                       context:(void *)context
{
    if (context == OSXMeisheReeditPresentationContext) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (self.sessionFinished || self.exporting || self.reeditProjectId.length == 0
                || self.reeditPresenter.presentedViewController) {
                return;
            }
            [self finishCancelled];
        });
        return;
    }
    if (context == OSXMeisheInstanceStateContext) {
        NSNumber *state = [change[NSKeyValueChangeNewKey] isKindOfClass:NSNumber.class]
            ? change[NSKeyValueChangeNewKey]
            : nil;
        if (state.integerValue == DCUniInstanceDestroy) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self finishCancelled];
            });
        }
        return;
    }
    [super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
}

- (void)stopWatchingReeditDismissal
{
    if (self.observingReeditPresentation && self.reeditPresenter) {
        [self.reeditPresenter removeObserver:self
                                  forKeyPath:@"presentedViewController"
                                     context:OSXMeisheReeditPresentationContext];
    }
    self.observingReeditPresentation = NO;
    self.reeditPresenter = nil;
}

- (void)stopWatchingInstanceDestruction
{
    if (self.observingInstanceState && self.observedInstance) {
        [self.observedInstance removeObserver:self
                                   forKeyPath:@"uniState"
                                      context:OSXMeisheInstanceStateContext];
    }
    self.observingInstanceState = NO;
    self.observedInstance = nil;
}

- (void)publishWithProjectId:(NSString *)projectId
              coverImagePath:(NSString *)coverImagePath
                    hasDraft:(BOOL)hasDraft
                   videoPath:(NSString *)videoPath
                 description:(NSString *)description
                   videoEdit:(UINavigationController *)videoEditNavigationController
{
    if (self.sessionFinished || self.exporting) return;
    if (![self timelineDurationAllowed]) {
        [self showEditedDurationErrorOn:videoEditNavigationController];
        return;
    }
    self.exporting = YES;
    BOOL compileStarted = [self.moduleManager compileCurrentTimeline];
    if (!compileStarted) {
        self.exporting = NO;
        [self finishFailure:@"compile_start_failed" message:@"视频发布任务启动失败"];
    }
}

- (void)didCompileFloatProgress:(float)progress
{
    // The uni-app bridge has a single terminal callback contract; progress remains inside the native editor.
}

- (void)didCompileCompleted:(NSString *)outputPath error:(NSError *)error
{
    self.exporting = NO;
    if (error || outputPath.length == 0 || ![[NSFileManager defaultManager] fileExistsAtPath:outputPath]) {
        [self finishFailure:@"compile_failed" message:(error.localizedDescription ?: @"视频导出失败")];
        return;
    }
    if (self.maxTime > 0 && [self videoDuration:outputPath] > self.maxTime) {
        [self showEditedDurationErrorOn:nil];
        return;
    }
    self.completingSuccess = YES;
    UIViewController *presenter = [self presenter];
    if (presenter) {
        // `presenter` is the visible SDK editor in the usual case. Dismissing it
        // directly also dismisses any export sheet it owns before JS navigates.
        [presenter dismissViewControllerAnimated:YES completion:^{
            [self finishSuccess:outputPath];
        }];
    } else {
        [self finishSuccess:outputPath];
    }
}

- (BOOL)timelineDurationAllowed
{
    if (self.maxTime <= 0 || !self.moduleManager.editManager.timeline) return YES;
    return self.moduleManager.editManager.timeline.duration <= (int64_t)(self.maxTime * 1000000.0);
}

- (void)showEditedDurationErrorOn:(UIViewController *)viewController
{
    UIViewController *presenter = viewController ?: [self presenter];
    if (!presenter) {
        [self finishFailure:@"duration_exceeded" message:@"编辑后视频仍超过时长限制"];
        return;
    }
    NSString *message = [NSString stringWithFormat:@"编辑后视频仍超过 %.0f 秒，请继续裁剪后再发布", self.maxTime];
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"视频时长超限"
                                                                   message:message
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"继续编辑" style:UIAlertActionStyleDefault handler:nil]];
    [presenter presentViewController:alert animated:YES completion:nil];
}

- (NSString *)copyVideoToCache:(NSURL *)sourceURL
{
    NSString *extension = sourceURL.pathExtension.length > 0 ? sourceURL.pathExtension : @"mp4";
    NSString *directory = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Caches/OSX-MeisheVideo"];
    NSError *error = nil;
    [[NSFileManager defaultManager] createDirectoryAtPath:directory withIntermediateDirectories:YES attributes:nil error:&error];
    if (error) return nil;
    NSString *destination = [directory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.%@", NSUUID.UUID.UUIDString, extension]];
    if (![[NSFileManager defaultManager] copyItemAtURL:sourceURL toURL:[NSURL fileURLWithPath:destination] error:&error]) {
        return nil;
    }
    return destination;
}

- (NSTimeInterval)videoDuration:(NSString *)path
{
    AVURLAsset *asset = [AVURLAsset URLAssetWithURL:[NSURL fileURLWithPath:path] options:nil];
    CMTime duration = asset.duration;
    if (!CMTIME_IS_NUMERIC(duration)) return 0;
    NSTimeInterval seconds = CMTimeGetSeconds(duration);
    return isfinite(seconds) && seconds > 0 ? seconds : 0;
}

- (UIViewController *)presenter
{
    UIViewController *controller = self.uniInstance.viewController;
    if (!controller) controller = NvSPUtils.keyWindow.rootViewController;
    while (controller.presentedViewController && !controller.presentedViewController.isBeingDismissed) {
        controller = controller.presentedViewController;
    }
    return controller;
}

- (NSString *)stringValue:(id)value
{
    return [value isKindOfClass:NSString.class] ? value : @"";
}

- (NSTimeInterval)numberValue:(id)value
{
    return [value respondsToSelector:@selector(doubleValue)] ? [value doubleValue] : 0;
}

- (void)finishSuccess:(NSString *)path
{
    [self finishOnce:@{ @"outputFilePath": path ?: @"", @"sessionId": self.sessionId ?: @"" }];
}

- (void)finishCancelled
{
    [self finishOnce:@{ @"cancelled": @YES, @"sessionId": self.sessionId ?: @"" }];
}

- (void)finishFailure:(NSString *)code message:(NSString *)message
{
    [self finishOnce:@{ @"errorCode": code ?: @"unknown_error", @"errorMessage": message ?: @"视频编辑失败", @"sessionId": self.sessionId ?: @"" }];
}

- (void)finishOnce:(NSDictionary *)result
{
    UniModuleKeepAliveCallback callback = nil;
    @synchronized (OSXMeisheVideoModule.class) {
        if (self.sessionFinished) return;
        self.sessionFinished = YES;
        self.exporting = NO;
        self.completingSuccess = NO;
        callback = self.sessionCallback;
        self.sessionCallback = nil;
        self.picker.delegate = nil;
        self.picker = nil;
        self.moduleManager.delegate = nil;
        self.moduleManager.compileDelegate = nil;
        [self stopWatchingReeditDismissal];
        [self stopWatchingInstanceDestruction];
        if (self.reeditProjectId.length > 0) {
            [self.moduleManager exitVideoEdit:self.reeditProjectId];
            self.reeditProjectId = nil;
        }
        if (OSXMeisheActiveModule == self) OSXMeisheActiveModule = nil;
    }
    [self invoke:callback result:result];
}

- (void)invoke:(UniModuleKeepAliveCallback)callback result:(NSDictionary *)result
{
    if (!callback) return;
    dispatch_async(dispatch_get_main_queue(), ^{
        callback(result ?: @{}, NO);
    });
}

- (void)dealloc
{
    [self finishCancelled];
}

@end
