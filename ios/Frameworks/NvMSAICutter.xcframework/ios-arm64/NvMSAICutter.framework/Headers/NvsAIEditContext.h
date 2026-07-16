//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 18. 2022
//   Author:        Meishe video team
//================================================================================

#import <Foundation/Foundation.h>
#import <NvStreamingSdkCore/NvsObject.h>
#import "NvsAICommon.h"

NS_ASSUME_NONNULL_BEGIN


// Main AI Edit Context interface
NVS_EXPORT @interface NvsAIEditContext : NSObject

// Singleton pattern
+ (instancetype)sharedInstance;
+ (void)destroyInstance;

// Static methods for global information
+ (NSString *)getAIEditVersionStr;
+ (NvsAITaskEvent *)getTaskEventByName:(NSString *)name eventType:(NvsAIEventType)eventType;
+ (NSString *)parseLandMarkByLongitude:(float)longitude latitude:(float)latitude;
+ (double)getEventGlobalParamInner:(NSString *)key defaultVal:(double)defaultVal;

// Getters for global data
+ (NSArray<NvsAITaskEvent *> *)getShotEventList;
+ (NSArray<NvsAITaskEvent *> *)getMatchEventList;
+ (NSArray<NvsLandMarkInfo *> *)getLandMarkInfoList;
+ (NSDictionary<NSString *, NSArray<NSString *> *> *)getPreferEventMap;
+ (NSArray<NvsAIGCCaptionTagMap *> *)getAigcCapTagMap;
+ (NSArray<NvsAIGCCaption *> *)getAigcCapList;

// Instance methods
+ (BOOL)verifySdkLicenseFile:(NSString *)licPath;
- (BOOL)readTaskEventsFromPath:(NSString *)filePath globalEventParamsPath:(NSString *)globalEventParamsPath;
- (BOOL)readAIGCCaptionsFromPath:(NSString *)filePath;
- (void)installDescData:(NSString *)filePath;

@end

NS_ASSUME_NONNULL_END


