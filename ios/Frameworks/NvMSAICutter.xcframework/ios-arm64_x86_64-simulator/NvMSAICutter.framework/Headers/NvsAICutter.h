//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 18. 2025
//   Author:        Meishe video team
//================================================================================

#import <Foundation/Foundation.h>
#import <NvStreamingSdkCore/NvsObject.h>
#import "NvsAICommon.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NvsRecommandMatchType) {
    NvsRecommandMatchTypeCommon = 0,
    NvsRecommandMatchTypeSmartRich = 1, // 智片丰富运镜
    NvsRecommandMatchTypeSmartCommon, // 智片普通运镜
    NvsRecommandMatchTypeIntelligent, // 一键成片
};

@protocol NvsAICutterDelegate <NSObject>
@optional

- (void)matchTemplateProgress:(int)progress type:(NSString *)type;

@end

NVS_EXPORT @interface NvsAICutter : NvsObject

@property (nonatomic, weak) id<NvsAICutterDelegate> delegate;

- (instancetype)init;

- (BOOL)readTemplates:(NSArray<NSString *>*)templateJsons;
- (void)clearTemplates;
- (void)setDisableIDs:(NSArray<NSString *> *)templateIDs;
- (void)clearDisableIDs;

- (NSArray<NvsAIFrameTag *> *)getTagInfos:(NSArray<NSString *> *)paths;

- (void)setAigcBlankAessts:(NSString *)assetPath videoPaths:(NSArray<NSString *> *)videoPaths;

- (void)setPathsForFormula:(NSArray<NSString *> *)paths;

- (NSArray<NvsTimeStampValue *> *)getFormulaValue:(NvsAICutFormula *)formula
                                             path:(NSString *)path
                                   startTimeStamp:(uint64_t)startTimeStamp
                                     endTimeStamp:(uint64_t)endTimeStamp;

- (NSArray<NSString *> *)recommandVideosForCutSame:(NSString *)templateId
                                         dataPaths:(NSArray<NSString *> *)dataPaths
                                        videoPaths:(NSArray<NvsVideoInfo *> *)videoInfos;
//  AI cut same function
- (NvsClipInfo *_Nullable)matchTemplateCutSame:(NSString *)templateId
                                     dataPaths:(NSArray<NSString *> *)dataPaths
                                    videoPaths:(NSArray<NvsVideoInfo *> *)videoInfos;

- (NvsClipInfo *_Nullable)matchTemplateCutSame:(NSString *)longTemplateId
                               shortTemplateId:(NSString *)shortTemplateId
                                     dataPaths:(NSArray<NSString *> *)dataPaths;

- (NvsClipInfo *_Nullable)matchTemplateCutSameForFixedSlots:(NSString *)templateId
                                               infoDataList:(NSArray<NvsVideoInfo *> *)infoDataList;

- (NSArray<NvsClipInfo *> *)matchTemplateCaptureSame:(NvsCaptureSame *)captureSame;

- (NSArray<NSString *> *)recommandTemplate:(NSArray<NvsVideoInfo *> *)infoDataPaths
                                videoInfos:(NSArray<NvsVideoInfo *> *)videoInfos
                            extroMatchInfo:(NvsExtroMatchInfo *_Nullable)extroMatchInfo;

- (NSArray<NSString *> *)recommandTemplate:(NSArray<NvsVideoInfo *> *)infoDataPaths
                                videoInfos:(NSArray<NvsVideoInfo *> *)videoInfos
                            extroMatchInfo:(NvsExtroMatchInfo *_Nullable)extroMatchInfo
                             templateCount:(int)templateCount
                                   doCheck:(BOOL)doCheck;

- (NSArray<NSString *> *)recommandTemplate:(NSArray<NvsVideoInfo *> *)infoDataPaths
                             templateCount:(int)templateCount
                                 matchType:(NvsRecommandMatchType)matchType;

- (NvsClipInfo *_Nullable)matchTemplate:(NSString *)templateId;
- (NvsClipInfo *_Nullable)matchTemplate:(NSString *)templateId matchType:(NvsRecommandMatchType)matchType;
- (NvsClipInfo *_Nullable)reMatchTemplate:(NvsClipInfo *)templateClipInfo;

- (NSArray<NvsClipInfo *> *)matchTemplateMemory:(NSArray<NSString *> *)dataPaths
                                         config:(NvsMatchMemoryConfig *)config;

- (BOOL)checkRecommandTemplate:(NSArray<NSString *> *)dataPaths
                    videoInfos:(NSArray<NvsVideoInfo *> *)videoInfos;

- (BOOL)replaceTemplateSlot:(NSString *)templateId
                  videoInfo:(NvsVideoInfo *)videoInfo
                   unitClip:(NvsUnitClip *)unitClip;

- (bool)addHudEffectOnTemplateSlot:(NSString *)hudJsonFile
                        templateId:(NSString *)templateId
                         videoInfo:(NvsVideoInfo *)videoInfo
                          unitClip:(NvsUnitClip *)unitClip;

@end

NS_ASSUME_NONNULL_END
